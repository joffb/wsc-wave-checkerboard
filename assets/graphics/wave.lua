local process = require("wf.api.v1.process")
local superfamiconv = require("wf.api.v1.process.tools.superfamiconv")

process.emit_symbol("gfx_wave", superfamiconv.convert_tilemap(
	"wave.png",
	superfamiconv.config()
		:mode("wsc")
        :bpp(4)
))