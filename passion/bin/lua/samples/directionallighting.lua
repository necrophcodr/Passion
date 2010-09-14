-- Configuration
function GAME:Config( cfg )
	-- Set window parameters.
	cfg.Title 				= "Example - Directional lighting"
	cfg.Width 			= 800
	cfg.Height 			= 600
	cfg.Fullscreen 	= false
	
	return cfg
end

function GAME:Initialize()	
	-- Alter render settings.
	render.SetDepthEnabled( true )
	render.SetTexturingEnabled( true )
	
	-- Load the model itself.
	self.Model = render.LoadModel( "models/teapot.obj" )
	
	-- Load lighting shader
	self.Shader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/light/directional_pixel.vs" ) ), render.CreatePixelShader( file.Read( "shaders/light/directional.ps" ) ) )
end

function GAME:Update( dt )
	-- We don't have to update anything.
end

function GAME:Draw()	
	-- Activate the lighting shader.
	render.SetProgram( self.Shader )
	
	-- Update light properties
	render.SetProgramFloat( "lightDiffuse", 1.0, 1.0, 1.0, 1.0 );
	render.SetProgramFloat( "lightSpecular", 1.0, 1.0, 1.0, 1.0 );
	render.SetProgramFloat( "lightAmbient", 1.0, 1.0, 1.0, 1.0 );
	render.SetProgramFloat( "lightPos", math.cos( os.clock() ) * 3, math.sin( os.clock() ) * 3, 3.0 );
	render.SetProgramFloat( "eyePos", math.cos( os.clock() / 10 ) * 5, math.sin( os.clock() / 10 ) * 5, 3 );
	render.SetProgramFloat( "diffuse", 1.0 );
	render.SetProgramFloat( "specular", 0.02 );
	render.SetProgramFloat( "shininess", 0.4 );
	render.SetProgramFloat( "ambient", 0.1 );
	
	-- Clear the color and depth buffer.
	render.Clear( Color( 10, 10, 10 ) )
	render.ClearZ()
	
	-- Start a 3D projection from the specified position
	render.Start3D( Vector( math.cos( os.clock() / 10 ) * 5, math.sin( os.clock() / 10 ) * 5, 3 ), Vector( 0, 0, 0 ) )				
		render.DrawModel( self.Model )
	render.End3D()
	
	-- Present the image to the screen.
	render.Present()
end

function GAME:Unload()
	-- Do nothing.
end
