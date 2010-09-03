-- Setup game
function GAME:Config( cfg )
	cfg.Title = "Example - 2D lighting"
	cfg.Width = 800
	cfg.Height = 600
	cfg.Fullscreen = false
	
	return cfg
end

function GAME:Initialize()
	render.SetTexturingEnabled( true )
	
	self.LightPos = Vector( 300, 350 )
	
	self.Shapes = {
		{
			Vector( 400, 300 ),
			Vector( 450, 300 ),
			Vector( 450, 350 )
		},
		{
			Vector( 200, 300 ), -- 1
			Vector( 230, 270 ), -- 2
			Vector( 260, 300 ), -- 3
			
			Vector( 260, 300 ), -- 4
			Vector( 260, 330 ), -- 5
			Vector( 230, 350 ), -- 6
			
			Vector( 230, 350 ), -- 7
			Vector( 200, 330 ), -- 8
			Vector( 200, 300 ), -- 9
			
			Vector( 200, 300 ), -- 10
			Vector( 260, 300 ), -- 11
			Vector( 230, 350 ) -- 12
		}
	}
	
	self.Sides = {
		{
			{ 1, 2 },
			{ 2, 3 },
			{ 3, 1 }
		},
		{
			{ 1, 2 },
			{ 2, 3 },
			{ 3, 5 },
			{ 5, 6 },
			{ 7, 8 },
			{ 8, 1 }
		}
	}
	
	for x = 1, 10 do
		local box = {}
		
		table.insert( box, Vector( 290 + x * 20, 200 ) )
		table.insert( box, Vector( 300 + x * 20, 200 ) )
		table.insert( box, Vector( 300 + x * 20, 210 ) )
		table.insert( box, Vector( 300 + x * 20, 210 ) )
		table.insert( box, Vector( 290 + x * 20, 210 ) )
		table.insert( box, Vector( 290 + x * 20, 200 ) )
		
		table.insert( self.Shapes, box )
		table.insert( self.Sides, { { 1, 2 }, { 2, 3 }, { 3, 5 }, { 5, 1 } } )
	end
	
	self.RT = render.CreateRenderTarget( 800, 600 )
	
	self.LightShader = render.CreateProgram( render.CreateVertexShader( file.Read( "shaders/null.vs" ) ), render.CreatePixelShader( file.Read( "shaders/2dlight.ps" ) ) )
end

function GAME:Update()
	if ( input.IsMouseDown( MOUSE_LEFT ) ) then
		self.LightPos = Vector( input.MouseX(), input.MouseY() )
	end
end

function GAME:CalculateShadowVolume( shape, ssides )
	-- Check which sides are lit
	local sides = {}
	
	for i = 1, #ssides do
		local p1, p2 = shape[ ssides[i][1] ], shape[ ssides[i][2] ]
		
		local n = ssides[i][3] or Vector( p1.y - p2.y, p2.x - p1.x ):Normal()
		local m = p1 + ( p2 - p1 ) / 2
		local d = n:Dot( m - self.LightPos )
		
		table.insert( sides, { p1, p2, d > 0 } )
	end
	
	-- Find light boundaries
	local boundary1
	for i = 1, #sides do
		local previ = i - 1
		if ( i == 1 ) then previ = #sides end
		
		if ( sides[previ][3] and sides[i][3] == false ) then
			boundary1 = i
			break
		end
	end
	
	-- Build shadow volume
	if ( boundary1 ) then
		local volume = {}
		local p1, p2, p3, p4
		
		for i = 1, #sides do
			if ( not sides[i][3] ) then
				p1 = sides[i][1]
				p2 = sides[i][1] + ( sides[i][1] - self.LightPos ):Normal() * 10000
				p3 = sides[i][2] + ( sides[i][2] - self.LightPos ):Normal() * 10000
				p4 = sides[i][2]
				
				table.insert( volume, p1 )
				table.insert( volume, p2 )
				table.insert( volume, p3 )
				table.insert( volume, p3 )
				table.insert( volume, p4 )
				table.insert( volume, p1 )
			end
		end
		
		return volume
	else
		return { Vector( 0, 0 ), Vector( 800, 0 ), Vector( 800, 600 ), Vector( 800, 600 ), Vector( 0, 600 ), Vector( 0, 0 ) }
	end
end

function GAME:DrawShape( shape )
	for i = 1, #shape, 3 do
		render.DrawTriangle( shape[i], shape[i+1], shape[i+2] )
	end
end

function GAME:Draw()
	render.SetRenderTarget( self.RT )
	render.SetTexture()
	render.SetProgram()
	
	render.Clear( Color( 255, 255, 255 ) )
	
	render.Start2D()
		-- Draw shapes
		for i = 1, #self.Shapes do			
			render.SetDrawColor( Color( 255, 0, 0 ) )
			self:DrawShape( self.Shapes[i] )
		end
		
		-- Draw shadow volumes
		for i = 1, #self.Shapes do
			render.SetDrawColor( Color( 0, 0, 0 ) )
			self:DrawShape( self:CalculateShadowVolume( self.Shapes[i], self.Sides[i] )	)
		end
	render.End2D()
	
	render.SetRenderTarget()
	render.SetTexture( self.RT:GetTexture() )
	render.SetProgram( self.LightShader )
	render.SetDepthEnabled( false )
	
	render.SetProgramFloat( "lightPos", self.LightPos.x / 800, 1.0 - self.LightPos.y / 600 )
	render.SetProgramFloat( "lightColor", 0.8, 0.8, 1.0, 0.6 )
	
	render.Start2D()
		render.SetDrawColor( Color( 255, 255, 255 ) )
		render.DrawRect( 0, 600, 800, -600 )
	render.End2D()
	
	render.Present()
end