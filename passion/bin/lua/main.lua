function GAME:Config( cfg )	
	cfg.Title = "Test game"
	cfg.Width = 800
	cfg.Height = 600
	
	return cfg
end

function GAME:Initialize()
	
end

function GAME:Update( dt )
	
end

function GAME:Draw()
	render.Clear( Color( math.sin( os.clock() ) * 127 + 127, 0, 0 ) )
	
	render.Present()
end

function GAME:Unload()
	
end