-- Include the configuration
dofile( "lua/config.lua" )

function GAME:Initialize()	
	-- Alter render settings
	render.SetDepthEnabled( true )
	render.SetTexturingEnabled( true )
	render.SetCullingEnabled( true )
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
	self.Program = render.CreateProgram( self.Shaders[1], self.Shaders[2] )
	
	-- Make the 'wave' the current program
	render.SetProgram( self.Program )
end

function GAME:Update( dt )
	-- Change the time variable in the shader to update the wave
	render.SetProgramFloat( "time", os.clock() )
end

function GAME:Draw()
	-- Clear the color and depth buffer
	if ( input.IsKeyDown( KEY_F1 ) ) then
		render.Clear( Color( 255, 0, 0 ) )
	else
		render.Clear( Color( 10, 10, 10 ) )
	end
	render.ClearZ()
	
	-- Start a 3D projection from the specified position, looking at 30 units above the center of the world
	render.Start3D( Vector( math.cos( os.clock() / 3 ) * 300, math.sin( os.clock() / 3 ) * 300, 200 ), Vector( 0, 0, 30 ) )		
		--render.SetDrawColor( Color( 255, 255, 255, 255 ) )
		
		-- Draw the tank model
		render.DrawModel( self.Model )
	render.End3D()
	
	-- Present the image to the screen
	render.Present()
end

function GAME:Unload()
	-- Do nothing
end