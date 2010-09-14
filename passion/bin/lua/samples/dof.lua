function GAME:Config( cfg )
	cfg.Title = "Depth of field"
	cfg.Width = 1280
	cfg.Height = 720
	
	return cfg
end

function GAME:Initialize()
	render.SetTexturingEnabled( true )
	
	self.Texture = render.LoadTexture( "textures/models/debug.png" )
	
	self.DepthWriterShader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/null.vs" ) ), render.CreatePixelShader( file.Read( "shaders/dof/depth.ps" ) ) )
	self.DepthReaderShader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/null.vs" ) ), render.CreatePixelShader( file.Read( "shaders/dof/visualdepth.ps" ) ) )
	
	self.BlurShaderV = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/null.vs" ) ), render.CreatePixelShader( file.Read( "shaders/dof/blur_v.ps" ) ) )
	self.BlurShaderH = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/null.vs" ) ), render.CreatePixelShader( file.Read( "shaders/dof/blur_h.ps" ) ) )
	
	self.RT = render.CreateRenderTarget( 1280, 720 )
	self.RT2 = render.CreateRenderTarget( 1280, 720 )
end

function GAME:Update()

end

function GAME:DrawScene()
	render.Start3D( Vector( 0, 20, 2 ), Vector( 0, 0, 0 ), 45, 0.1, 40 )
		for i = 1, 5 do
			local mat = Matrix()
			
			if ( i % 2 == 0 ) then			
				mat:Translate( Vector( -9 + i * 3, math.sin( os.clock() ) * 12, 0 ) )
			else
				mat:Translate( Vector( -9 + i * 3, math.cos( os.clock() ) * 12, 0 ) )
			end
			
			render.SetTransform( mat )
			
			render.DrawBox( Vector( -1, -1, -1 ), Vector( 1, 1, 1 ) )
		end
		
		render.SetTexture()
		render.SetTransform()
		
		render.DrawQuad( Vector( -30, -35, -1 ), Vector( 30, -35, -1 ), Vector( 30, 25, -1 ), Vector( -30, 25, -1 ) )
		render.DrawQuad( Vector( -30, -19, 5 ), Vector( 30, -19, 5 ), Vector( 30, -19, -1 ), Vector( -30, -19, -1 ) )
	render.End3D()
end

function GAME:Draw()
	-- First draw the scene itself
	render.SetRenderTarget( self.RT )
	render.SetDepthEnabled( true )
	render.SetDrawColor( Color( 255, 255, 255, 255 ) )
	
	render.Clear( Color( 0, 0, 0, 255 ) )
	render.ClearZ()
	
	render.SetTexture( self.Texture )
	render.SetProgram( self.DepthWriterShader )
	
	self:DrawScene()
	
	-- Blur pass 1
	render.SetRenderTarget( self.RT2 )
	render.SetDepthEnabled( false )
	
	render.SetProgram( self.BlurShaderV )
	render.SetTexture( self.RT:GetTexture() )
	
	render.Start2D()
		render.DrawRect( 0, 720, 1280, -720 )
	render.End2D()
	
	-- Blur pass 2
	render.SetRenderTarget()
	
	render.SetProgram( self.BlurShaderH )
	render.SetTexture( self.RT2:GetTexture() )
	
	render.Start2D()
		render.DrawRect( 0, 720, 1280, -720 )
	render.End2D()
	
	-- VIsualize depth buffer
	render.SetProgram( self.DepthReaderShader )
	render.SetTexture( self.RT:GetTexture() )
	
	render.Start2D()
		render.DrawRect( 1280 * 0.8, 144, 256, -144 )
	render.End2D()
	
	render.Present()
end