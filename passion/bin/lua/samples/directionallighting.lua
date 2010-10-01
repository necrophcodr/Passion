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
	self.Model = Model( "models/teapot.obj" )
	
	-- Load lighting shader
	self.Shader = Shader( file.Read( "shaders/light/directional_pixel.vs" ), file.Read( "shaders/light/directional.ps" ) )
end

function GAME:Update( dt )
	-- We don't have to update anything.
end

function GAME:Draw()	
	-- Activate the lighting shader.
	render.SetShader( self.Shader )
	
	-- Update light properties
	self.Shader:SetFloat( "lightDiffuse", 1.0, 1.0, 1.0, 1.0 );
	self.Shader:SetFloat( "lightSpecular", 1.0, 1.0, 1.0, 1.0 );
	self.Shader:SetFloat( "lightAmbient", 1.0, 1.0, 1.0, 1.0 );
	self.Shader:SetFloat( "lightPos", math.cos( os.clock() ) * 3, math.sin( os.clock() ) * 3, 3.0 );
	self.Shader:SetFloat( "eyePos", math.cos( os.clock() / 10 ) * 5, math.sin( os.clock() / 10 ) * 5, 3 );
	self.Shader:SetFloat( "diffuse", 1.0 );
	self.Shader:SetFloat( "specular", 0.02 );
	self.Shader:SetFloat( "shininess", 0.4 );
	self.Shader:SetFloat( "ambient", 0.1 );
	
	-- Clear the color and depth buffer.
	render.Clear( Color( 10, 10, 10 ) )
	render.ClearZ()
	
	-- Start a 3D projection from the specified position
	render.Start3D( Vector( math.cos( os.clock() / 10 ) * 4, math.sin( os.clock() / 10 ) * 4, 3 ), Vector( 0, 0, 0 ) )				
		self.Model:Draw()
	render.End3D()
end

function GAME:Unload()
	-- Do nothing.
end
