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
	self.Model = render.LoadModel( "models/ssao_scene.obj" )
	
	-- Load noise texture
	self.NoiseTexture = render.LoadTexture( "textures/noise.png" )
	
	-- Load shaders
	self.NormalDepthShader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/ssao/normaldepth.vs" ) ), render.CreatePixelShader( file.Read( "shaders/ssao/normaldepth.ps" ) ) )
	self.SSAOShader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/ssao/ssao.vs" ) ), render.CreatePixelShader( file.Read( "shaders/ssao/ssao.ps" ) ) )
	self.LightingShader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/light/directional_pixel.vs" ) ), render.CreatePixelShader( file.Read( "shaders/light/directional.ps" ) ) )
	self.FinalShader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/ssao/final.vs" ) ), render.CreatePixelShader( file.Read( "shaders/ssao/final.ps" ) ) )
	
	-- Create render targets
	self.RTNormalDepth = render.CreateRenderTarget( 1280, 720 )
	self.RTSSAO = render.CreateRenderTarget( 1280, 720 )
	self.RTGeometry = render.CreateRenderTarget( 1280, 720 )
	
	-- Set up SSAO shader
	render.SetProgram( self.SSAOShader )
	render.SetProgramFloat( "offset", 18 )
	render.SetProgramFloat( "rad", 0.00497 )
	render.SetProgramFloat( "strength", 0.07840 )
	render.SetProgramFloat( "totStrength", 1.18000 )
	render.SetProgramFloat( "falloff", 0 )
	
	render.SetProgramInt( "Texture0", 0 )
	render.SetProgramInt( "rnm", 1 )
	
	-- Set up lighting shader
	render.SetProgram( self.LightingShader )
	render.SetProgramFloat( "lightDiffuse", 1.0, 1.0, 1.0, 1.0 )
	render.SetProgramFloat( "lightSpecular", 1.0, 1.0, 1.0, 1.0 )
	render.SetProgramFloat( "lightAmbient", 1.0, 1.0, 1.0, 1.0 )
	render.SetProgramFloat( "lightPos", -37, -35, 30 )
	render.SetProgramFloat( "eyePos", -50, -40, 30  )
	render.SetProgramFloat( "diffuse", 1.0 )
	render.SetProgramFloat( "specular", 0.02 )
	render.SetProgramFloat( "shininess", 0.4 )
	render.SetProgramFloat( "ambient", 0.3 )
	
	-- Set up final shader
	render.SetProgram( self.FinalShader )
	render.SetProgramInt( "Texture0", 0 )
	render.SetProgramInt( "Texture1", 1 )
	
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
	
	render.SetProgram( self.NormalDepthShader )
	
	render.Clear( Color( 0, 0, 0 ) )
	render.ClearZ()
	
	render.Start3D( self.Pos, self.LookAt, 45, 0.1, 200 )
		render.DrawModel( self.Model )
	render.End3D()
	
	-- Pass #2 - SSAO
	render.SetRenderTarget( self.RTSSAO )
	
	render.SetProgram( self.SSAOShader )
	
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
	
	render.SetProgram( self.LightingShader )
	
	render.Clear( Color( 0, 0, 0 ) )
	render.ClearZ()
	
	render.Start3D( self.Pos, self.LookAt, 45, 0.1, 200 )
		render.DrawModel( self.Model )
	render.End3D()
	
	-- Final pass
	render.SetRenderTarget()
	
	render.SetProgram( self.FinalShader )
	
	render.Clear( Color( 0, 0, 0 ) )
	render.ClearZ()
	
	render.Start2D()
		render.SetTexture( self.RTGeometry:GetTexture(), 0 )
		render.SetTexture( self.RTSSAO:GetTexture(), 1 )
		
		render.DrawRect( 0, 720, 1280, -720 )
		
		render.SetTexture( 0, 0 )
		render.SetTexture( 0, 1 )
	render.End2D()
	
	render.Present( true )
end