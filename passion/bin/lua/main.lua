function GAME:Config( cfg )	
	cfg.Title = "Test game"
	cfg.Width = 800
	cfg.Height = 600
	
	return cfg
end

function GAME:Initialize()	
	render.SetDepthEnabled( true )
	render.SetTexturingEnabled( true )
	render.SetCullingEnabled( true )
	
	self.Texture = render.LoadTexture( "textures/models/tank.jpg" )
	render.SetTexture( self.Texture )
	
	self.Model = render.LoadModel( "models/tank.obj" )
	
	self.Shaders = {}
	self.Shaders[1] = render.CreateVertexShader( file.Read( "shaders/wave.vs" ) )
	self.Shaders[2] = render.CreatePixelShader( file.Read( "shaders/null.ps" ) )
	self.Program = render.CreateProgram( self.Shaders[1], self.Shaders[2] )
	render.SetProgram( self.Program )
end

function GAME:Update( dt )
	render.SetProgramFloat( "time", os.clock() )
end

function GAME:Draw()
	render.Clear( Color( 0, 162, 232 ) )
	render.ClearZ()
	
	render.Start3D( Vector( math.cos( os.clock() / 3 ) * 300, math.sin( os.clock() / 3 ) * 300, 200 ), Vector( 0, 0, 30 ) )		
		render.SetDrawColor( Color( 255, 255, 255, 255 ) )
		render.DrawModel( self.Model )
	render.End3D()
	
	render.Present()
end

function GAME:Unload()
	
end