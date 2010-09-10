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
	self.Texture = render.LoadTexture( "textures/models/tank.jpg" )
	
	-- Load the tank model itself.
	self.Model = render.LoadModel( "models/tank.obj" )
end

function GAME:Update( dt )
	-- We don't have to update anything.
end

function GAME:Draw()	
	-- Activate the texture.
	render.SetTexture( self.Texture )
	
	-- Clear the color and depth buffer.
	render.Clear( Color( 10, 10, 10 ) )
	render.ClearZ()
	
	-- Start a 3D projection from the specified position, looking at 30 units above the center of the world.
	render.Start3D( Vector( math.cos( os.clock() / 3 ) * 300, math.sin( os.clock() / 3 ) * 300, 200 ), Vector( 0, 0, 30 ) )				
		render.DrawModel( self.Model )
	render.End3D()
	
	-- Present the image to the screen.
	render.Present()
end

function GAME:Unload()
	-- Do nothing.
end
