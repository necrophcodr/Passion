////////////////////////////////////////////////////////////
//
//  Copyright (C) 2010 Alexander Overvoorde (overv161@gmail.com)
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// file.lua - Contains file functions
////////////////////////////////////////////////////////////

const char* default_lua =
"function GAME:Initialize() \
	render.SetAlphaBlendingEnabled( true ) \
	 \
	self.TexCloud = Texture( \"textures/cloud.png\" ) \
	self.TexLogo = Texture( \"textures/logo.png\" ) \
	self.TexGradient = Texture( \"textures/gradient.png\" ) \
	 \
	self.Clouds = {} \
	for x = 1, 3 do \
		self.Clouds[x] = {} \
		for y = 1, 3 do \
			self.Clouds[x][y] = Vector( -480 + x * 500 + math.random( 250 ), -270 + y * 290 + math.random( 150 ) ) \
		end \
	end \
end \
 \
function GAME:Update() \
	for x = 1, 3 do \
		for y = 1, 3 do \
			if ( self.Clouds[x][y].x > 1500 ) then self.Clouds[x][y].x = -600 end \
			self.Clouds[x][y].x = self.Clouds[x][y].x + 1 \
		end \
	end \
end \
 \
function GAME:Draw() \
	render.Clear( Color( 0, 0, 0 ) ) \
	 \
	 render.SetTransform() \
	render.SetTexture( self.TexGradient ) \
	 \
	render.Start2D() \
		render.DrawRect( 0, 0, 1280, 720 ) \
	render.End2D() \
	 \
	render.Start3D( Vector( 700 + math.sin( os.clock() / 3 ) * 100, 600, 500 ), Vector( 640, 400, 0 ), 45, 1, 10000, Vector( 0, 1, 0 ) ) \
		render.SetTexture( self.TexLogo ) \
		render.SetDrawColor( Color( 255, 255, 255, 128 ) ) \
		render.DrawRect( 392, 470, 496, -158 ) \
		 \
		render.SetDrawColor( Color( 255, 255, 255 ) ) \
		render.SetTexture( self.TexCloud ) \
		 \
		for x = 1, 3 do \
			for y = 1, 3 do \
				local depth = Matrix() \
				depth:Translate( Vector( 0, 0, math.sin( ( self.Clouds[x][y].x + self.Clouds[x][y].y ) / 200 ) * 20 ) ) \
				render.SetTransform() \
				 \
				 render.DrawRect( self.Clouds[x][y].x, self.Clouds[x][y].y + math.sin( self.Clouds[x][y].x / 10 ), 243 + math.sin( ( self.Clouds[x][y].x + self.Clouds[x][y].y ) / 50 ) * 10, 135 + math.sin( ( self.Clouds[x][y].x + self.Clouds[x][y].y ) / 50 ) * 5 ) \
			end \
		end\
	render.End3D(); \
end \
";