-- Include the configuration
dofile( "lua/config.lua" )

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
	
	self.RT = render.CreateRenderTarget( 1280, 720 )
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
	render.Start3D( Vector( math.cos( os.clock() / 3 ) * 300, math.sin( os.clock() / 3 ) * 300, 200 ), Vector( 0, 0, 30 ) )				
		-- Draw the tank model
		render.DrawModel( self.Model )
	render.End3D()
	
	-- Draw the render target 4 times, in each corner of the screen
	render.SetRenderTarget()
	render.SetTexture( self.RT )
	render.SetProgram()
	render.SetDepthEnabled( false )
	
	render.Start2D()
		render.SetDrawColor( 255, 255, 255, 255 )
		render.DrawQuad( Vector( 0, 360 ), Vector( 640, 360 ), Vector( 640, 0 ), Vector( 0, 0 ) )
		render.DrawQuad( Vector( 640, 360 ), Vector( 1280, 360 ), Vector( 1280, 0 ), Vector( 640, 0 ) )
		render.DrawQuad( Vector( 0, 720 ), Vector( 640, 720 ), Vector( 640, 360 ), Vector( 0, 360 ) )
		render.DrawQuad( Vector( 640, 720 ), Vector( 1280, 720 ), Vector( 1280, 360 ), Vector( 640, 360 ) )
	render.End2D()
	
	-- Present the image to the screen
	render.Present()
end

function GAME:Unload()
	-- Do nothing
end