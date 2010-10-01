function GAME:Config( cfg )
	cfg.Title = "Example - SSAO"
	cfg.Width = 1280
	cfg.Height = 720
	
	return cfg
end

function GAME:Initialize()
	-- Set up renderer
	render.SetDepthEnabled( true )
	render.SetCullingEnabled( true )
	
	-- Load scene
	self.Model = Model( "models/ssao_scene.obj" )
	
	-- Load noise texture
	self.NoiseTexture = Texture( "textures/noise.png" )
	
	-- Load shaders
	self.NormalDepthShader = Shader( file.Read( "shaders/ssao/normaldepth.vs" ), file.Read( "shaders/ssao/normaldepth.ps" ) )
	self.SSAOShader = Shader( file.Read( "shaders/ssao/ssao.vs" ), file.Read( "shaders/ssao/ssao.ps" ) )
	self.LightingShader = Shader( file.Read( "shaders/light/directional_pixel.vs" ), file.Read( "shaders/light/directional.ps" ) )
	self.FinalShader = Shader( file.Read( "shaders/ssao/final.vs" ), file.Read( "shaders/ssao/final.ps" ) )
	
	-- Create render targets
	self.RTNormalDepth = RenderTarget( 1280, 720 )
	self.RTSSAO = RenderTarget( 1280, 720 )
	self.RTGeometry = RenderTarget( 1280, 720 )
	
	-- Set up SSAO shader
	render.SetShader( self.SSAOShader )
	self.SSAOShader:SetFloat( "offset", 18 )
	self.SSAOShader:SetFloat( "rad", 0.00497 )
	self.SSAOShader:SetFloat( "strength", 0.07840 )
	self.SSAOShader:SetFloat( "totStrength", 1.18000 )
	self.SSAOShader:SetFloat( "falloff", 0 )
	
	self.SSAOShader:SetInt( "Texture0", 0 )
	self.SSAOShader:SetInt( "rnm", 1 )
	
	-- Set up lighting shader
	render.SetShader( self.LightingShader )
	self.LightingShader:SetFloat( "lightDiffuse", 1.0, 1.0, 1.0, 1.0 )
	self.LightingShader:SetFloat( "lightSpecular", 1.0, 1.0, 1.0, 1.0 )
	self.LightingShader:SetFloat( "lightAmbient", 1.0, 1.0, 1.0, 1.0 )
	self.LightingShader:SetFloat( "lightPos", -37, -35, 30 )
	self.LightingShader:SetFloat( "eyePos", -50, -40, 30  )
	self.LightingShader:SetFloat( "diffuse", 1.0 )
	self.LightingShader:SetFloat( "specular", 0.02 )
	self.LightingShader:SetFloat( "shininess", 0.4 )
	self.LightingShader:SetFloat( "ambient", 0.3 )
	
	-- Set up final shader
	render.SetShader( self.FinalShader )
	self.FinalShader:SetInt( "Texture0", 0 )
	self.FinalShader:SetInt( "Texture1", 1 )
	
	self.Pos = Vector( -30, -40, 10 )
	self.Yaw = math.pi / 3
	self.Dir = Vector( math.cos( self.Yaw ), math.sin( self.Yaw ), 0 )
	
	self.LastX = input.MouseX()
	self.LastState = false
end

function GAME:Update()
	if ( self.NextUpdate and os.clock() < self.NextUpdate ) then return end
	self.NextUpdate = os.clock() + 1/60
	
	if ( input.IsKeyDown( KEY_W ) ) then
		self.Pos = self.Pos + self.Dir
	elseif ( input.IsKeyDown( KEY_S ) ) then
		self.Pos = self.Pos - self.Dir
	end
	
	if ( input.IsKeyDown( KEY_A ) ) then
		self.Dir = Vector( math.cos( self.Yaw + math.pi / 2 ), math.sin( self.Yaw + math.pi / 2 ), 0 )
		self.Pos = self.Pos + self.Dir
	elseif ( input.IsKeyDown( KEY_D) ) then
		self.Dir = Vector( math.cos( self.Yaw - math.pi / 2 ), math.sin( self.Yaw - math.pi / 2 ), 0 )
		self.Pos = self.Pos + self.Dir
	end
	
	if ( input.IsMouseDown( MOUSE_LEFT ) and self.LastState == false ) then
		self.LastX = input.MouseX()
		self.LastState = true
	elseif ( input.IsMouseDown( MOUSE_LEFT ) ) then
		if ( input.MouseX() ~= self.LastX ) then
			self.Yaw = self.Yaw + ( self.LastX - input.MouseX() ) / 200
			self.LastX = input.MouseX()
		end
	else
		self.LastState = false
	end
	
	self.Dir = Vector( math.cos( self.Yaw ), math.sin( self.Yaw ), 0 )
	self.LookAt = self.Pos + self.Dir
end

function GAME:Draw()	
	-- Pass #1 - Normal/Depth
	render.SetRenderTarget( self.RTNormalDepth )
	
	render.SetShader( self.NormalDepthShader )
	
	render.Clear( Color( 0, 0, 0 ) )
	render.ClearZ()
	
	render.Start3D( self.Pos, self.LookAt, 45, 0.1, 200 )
		self.Model:Draw()
	render.End3D()
	
	-- Pass #2 - SSAO
	render.SetRenderTarget( self.RTSSAO )
	
	render.SetShader( self.SSAOShader )
	
	render.Clear( Color( 0, 0, 0 ) )
	render.ClearZ()
	
	render.Start2D()
		render.SetTexture( self.RTNormalDepth:GetTexture(), 0 )
		render.SetTexture( self.NoiseTexture, 1 )
		
		render.DrawRect( 0, 720, 1280, -720 )
		
		render.SetTexture( 0, 0 )
		render.SetTexture( 0, 1 )
	render.End2D()
	
	-- Pass #3 - Geometry
	render.SetRenderTarget( self.RTGeometry )
	
	render.SetShader( self.LightingShader )
	
	render.Clear( Color( 0, 0, 0 ) )
	render.ClearZ()
	
	render.Start3D( self.Pos, self.LookAt, 45, 0.1, 200 )
		self.Model:Draw()
	render.End3D()
	
	-- Final pass
	render.SetRenderTarget()
	
	render.SetShader( self.FinalShader )
	
	render.Clear( Color( 0, 0, 0 ) )
	render.ClearZ()
	
	render.Start2D()
		render.SetTexture( self.RTGeometry:GetTexture(), 0 )
		render.SetTexture( self.RTSSAO:GetTexture(), 1 )
		
		render.DrawRect( 0, 720, 1280, -720 )
		
		render.SetTexture( 0, 0 )
		render.SetTexture( 0, 1 )
	render.End2D()
end