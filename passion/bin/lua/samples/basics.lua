-- Configuration
function GAME:Config( cfg )
	-- Set window parameters.
	cfg.Title 				= "Example - Basics"
	cfg.Width 			= 800
	cfg.Height 			= 600
	cfg.Fullscreen 	= false
	
	return cfg
end

function GAME:Initialize()	
	-- Alter render settings.
	render.SetDepthEnabled( true )
	render.SetTexturingEnabled( true )
	
	-- Load a texture for our tank.
	self.Texture = Texture( "textures/models/tank.jpg" )
	
	-- Load the tank model itself.
	self.Model = Model( "models/tank.obj" )
end

function GAME:Update()
	-- We don't have to update anything.
end

function GAME:Draw()	
	-- Activate the texture.
	render.SetTexture( self.Texture )
	
	-- Clear the color and depth buffer.
	render.Clear( Color( 10, 10, 10 ) )
	render.ClearZ()
	
	-- Start a 3D projection from the specified position, looking at 30 units above the center of the world.
	render.Start3D( Vector( math.cos( os.clock() / 3 ) * 250, math.sin( os.clock() / 3 ) * 250, 150 ), Vector( 0, 0, 30 ) )
		self.Model:Draw()
	render.End3D()
end

function GAME:Unload()
	-- Do nothing.
end
