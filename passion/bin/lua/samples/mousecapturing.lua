function GAME:Config( cfg )
	cfg.Width = 800
	cfg.Height = 600
	cfg.Title = "Example - Mouse capturing"
	
	return cfg
end

function GAME:Initialize()
	render.SetAlphaBlendingEnabled( true )
	
	input.SetMousePos( 400, 300 )
	input.ShowMouseCursor( false )
	
	self.pos = { x = 400, y = 300 }
	self.captured = true
	self.spacestate = false
end

function GAME:Update()
	local mx, my = input.MouseX(), input.MouseY()
	
	if ( self.spacestate ~= input.IsKeyDown( KEY_SPACE ) and not self.spacestate ) then
		self.captured = not self.captured
		input.ShowMouseCursor( not self.captured )
		
		if ( self.captured ) then
			mx = 400
			my = 300
		end
	end
	self.spacestate = input.IsKeyDown( KEY_SPACE )
	
	if ( self.captured ) then
		
		local dx, dy = mx - 400, my - 300
		
		self.pos.x = self.pos.x + dx / 10
		self.pos.y = self.pos.y + dy / 10
		
		input.SetMousePos( 400, 300 )
	end
end

function GAME:Draw()
	render.Clear( Color( 0, 0, 0 ) )
	
	render.Start2D()
		render.SetDrawColor( Color( 255, 255, 255 ) )
		render.DrawText( 20, 20, "Press SPACE to toggle between capturing the cursor" )
		
		render.SetDrawColor( Color( 255, 0, 0 ) )
		render.SetTexture()
		render.DrawRect( self.pos.x, self.pos.y, 10, 10 )
	render.End2D()
end
