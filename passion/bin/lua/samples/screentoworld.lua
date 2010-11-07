function GAME:Config( cfg )
	cfg.Title 			= "Example - Screen to world"
	cfg.Width 			= 800
	cfg.Height 			= 600
	cfg.Fullscreen 	= false
	
	return cfg
end

function GAME:Initialize()	
	render.SetDepthEnabled( true )
	
	self.Rectangles = {}
	self.RectangleC = 0
end

function GAME:Update()
	if ( input.IsMouseDown( MOUSE_LEFT ) and not self.Drawing ) then
		self.Drawing = true
		self.StartPos = self.AimPos
	elseif ( input.IsMouseDown( MOUSE_LEFT ) == false and self.Drawing ) then
		self.Drawing = false
		
		local sx, sy, ex, ey
		if ( self.StartPos.x > self.AimPos.x ) then sx = self.AimPos.x ex = self.StartPos.x else sx = self.StartPos.x ex = self.AimPos.x end
		if ( self.StartPos.y > self.AimPos.y ) then sy = self.AimPos.y ey = self.StartPos.y else sy = self.StartPos.y ey = self.AimPos.y end
		
		self.Rectangles[self.RectangleC+1] = { Vector( sx, sy, 0 ), Vector( ex, sy, 0 ), Vector( ex, ey, 0 ), Vector( sx, ey, 0 ), sx, sy, ex, ey }
		self.RectangleC = self.RectangleC + 1
	end
	
	local p = self.AimPos
	for i, rect in pairs( self.Rectangles ) do
		if ( p.x > rect[5] and p.y > rect[6] and p.x < rect[7] and p.y < rect[8] ) then
			self.SelectedRect = i
			return
		end
	end
	self.SelectedRect = 0
end

function GAME:Draw()	
	render.Clear( Color( 153, 189, 144 ) )
	render.ClearZ()
	
	render.Start3D( Vector( 100, 100, 150 ), Vector( 0, 0, 0 ), 45, 1, 1000 )		
		render.SetDrawColor( Color( 255, 0, 0 ) )
		render.DrawLine( Vector( 0, 0, 0.5 ), Vector( 1000, 0, 0 ) )
		render.SetDrawColor( Color( 0, 200, 0 ) )
		render.DrawLine( Vector( 0, 0, 0.5 ), Vector( 0, 1000, 0 ) )
		render.SetDrawColor( Color( 0, 0, 255 ) )
		render.DrawLine( Vector( 0, 0, 0 ), Vector( 0, 0, 1000 ) )
		
		local n = render.ScreenToWorld( input.MouseX(), input.MouseY() )
		self.AimPos = Vector( 100, 100, 150 ) + n * 150 / -n.z
		
		if ( self.Drawing ) then
			render.SetDrawColor( Color( 0, 0, 0 ) )
			render.DrawLine( self.StartPos, Vector( self.AimPos.x, self.StartPos.y, self.StartPos.z ) )
			render.DrawLine( Vector( self.AimPos.x, self.StartPos.y, self.StartPos.z ), Vector( self.AimPos.x, self.AimPos.y, self.StartPos.z ) )
			render.DrawLine( Vector( self.AimPos.x, self.AimPos.y, self.StartPos.z ), Vector( self.StartPos.x, self.AimPos.y, self.StartPos.z ) )
			render.DrawLine( Vector( self.StartPos.x, self.AimPos.y, self.StartPos.z ), self.StartPos )
		end
		
		render.SetDrawColor( Color( 167, 172, 199 ) )
		
		for i, rect in pairs( self.Rectangles ) do
			if ( i == self.SelectedRect ) then
				render.SetDrawColor( Color( 167 * 1.1, 172 * 1.1, 199 * 1.1 ) )
					render.DrawQuad( rect[1],  rect[2],  rect[3],  rect[4] )
				render.SetDrawColor( Color( 167, 172, 199 ) )
			else
				render.DrawQuad( rect[1], rect[2], rect[3], rect[4] )
			end
		end
	render.End3D()
end

function GAME:UnLoad()

end