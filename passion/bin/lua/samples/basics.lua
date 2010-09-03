-- Configuration
function GAME:Config( cfg )
	-- Change window parameters
	cfg.Title 				= "Example - Basics"
	cfg.Width 			= 800
	cfg.Height 			= 600
	cfg.Fullscreen 	= false
	
	return cfg
end

function GAME:Initialize()	
	-- Alter render settings
	render.SetDepthEnabled( true )
	render.SetTexturingEnabled( true )
	--render.SetWireframeEnabled( true )
	--render.SetAlphaBlendingEnabled( true )
	
	-- Load a texture for our tank
	self.Texture = render.LoadTexture( "textures/models/tank.jpg" )
	render.SetTexture( self.Texture )
	
	-- Load the tank model itself
	self.Model = render.LoadModel( "models/tank.obj" )
	
	-- Create a shader that 'waves' our tank
	self.Shaders = {}
	self.Shaders[1] = render.CreateVertexShader( file.Read( "shaders/wave.vs" ) )
	self.Shaders[2] = render.CreatePixelShader( file.Read( "shaders/null.ps" ) )
	
	-- Programs consist out of a vertex shader and a pixel shader
	self.Wave = render.CreateProgram( self.Shaders[1], self.Shaders[2] )
	
	-- Make the 'wave' the current program
	render.SetProgram( self.Program )
	
	self.RT = render.CreateRenderTarget( 800, 600 )
end

function GAME:Update( dt )
end

function GAME:Draw()	
	-- Draw the main scene to a render target
	render.SetRenderTarget( self.RT )
	render.SetTexture( self.Texture )
	render.SetProgram( self.Wave )
	render.SetDepthEnabled( true )
	
	-- Clear the color and depth buffer
	render.Clear( Color( 10, 10, 10 ) )
	render.ClearZ()
	
	-- Change the time variable in the shader to update the wave
	render.SetProgramFloat( "time", os.clock() )
	
	-- Start a 3D projection from the specified position, looking at 30 units above the center of the world
	render.Start3D( Vector( math.cos( os.clock() / 3 ) * 500, math.sin( os.clock() / 3 ) * 500, 300 ), Vector( 0, 0, 30 ) )				
		-- Draw the tank model
		render.DrawModel( self.Model )
	render.End3D()
	
	-- Draw the render target 4 times, in each corner of the screen
	render.SetRenderTarget()
	render.SetTexture( self.RT:GetTexture() )
	render.SetProgram()
	render.SetDepthEnabled( false )
	render.SetTransform()
	
	render.Start2D()
		render.SetDrawColor( Color( 255, 255, 255, 255 ) )
		render.DrawQuad( Vector( 0, 300 ), Vector( 400, 300 ), Vector( 400, 0 ), Vector( 0, 0 ) )
		render.DrawQuad( Vector( 400, 300 ), Vector( 800, 300 ), Vector( 800, 0 ), Vector( 400, 0 ) )
		render.DrawQuad( Vector( 0, 600 ), Vector( 400, 600 ), Vector( 400, 300 ), Vector( 0, 300 ) )
		render.DrawQuad( Vector( 400, 600 ), Vector( 800, 600 ), Vector( 800, 300 ), Vector( 400, 300 ) )
	render.End2D()
	
	-- Present the image to the screen
	render.Present()
end

function GAME:Unload()
	-- Do nothing
end