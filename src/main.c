// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Adrian "asie" Siekierka, 2023
#include <wonderful.h>
#include <ws.h>
#include "sine.h"
#include "graphics/wave.h"


#ifdef IRAM_IMPLEMENTATION
#define IRAM_EXTERN
#else
#define IRAM_EXTERN extern
#endif

#define screen_1 ((ws_scr_entry_t __wf_iram*) 0x1000)
#define screen_2 ((ws_scr_entry_t __wf_iram*) 0x1800)
#define sprites ((ws_sprite_t __wf_iram*) 0x2e00)

uint8_t tic;
uint8_t palette_tic;

uint8_t scroll_ptr;
uint8_t scroll_tic;

uint8_t vblank_fired;

uint8_t scroll_y_values[256];

__attribute__((interrupt)) void hblank(void) __far
{
	// update y scroll from table
	outportb(IO_SCR1_SCRL_Y, scroll_y_values[inportb(IO_LCD_LINE)]);

	ws_hwint_ack(HWINT_HBLANK_TIMER);
}

__attribute__((interrupt)) void vblank(void) __far
{
	static uint8_t i, pos, checkerscroll, y_value;

	vblank_fired = 0x01;

	pos = tic;
	checkerscroll = -tic;

	// y scroll values for lines
	for (i = 0; i < 144; i++)
	{
		// sine values scaled to 0-63
		y_value = sine[pos];

		// correct for screen line offset
		y_value = y_value - i - 1;

		// every 32 lines add 64 to get alternate checkerboard squares
		// every 64 lines add 128 to get alternate colours
		y_value += (checkerscroll & (32 | 64)) << 1;

		scroll_y_values[i] = y_value;

		checkerscroll++;
		pos++;
	}

	tic++;

	ws_hwint_ack(HWINT_VBLANK);
}

void disable_interrupts()
{
	// disable cpu interrupts
	cpu_irq_disable();

	// disable wonderswan hardware interrupts
	ws_hwint_disable_all();
}

void enable_interrupts()
{
	// acknowledge interrupt
	outportb(IO_HWINT_ACK, 0xFF);

	// set interrupt handler which only acknowledges the vblank interrupt
	//ws_hwint_set_default_handler_vblank();
	//ws_hwint_set_default_handler_hblank_timer();	
	ws_hwint_set_handler(HWINT_IDX_VBLANK, vblank);
	ws_hwint_set_handler(HWINT_IDX_HBLANK_TIMER, hblank);
	
	outportb(IO_LCD_INTERRUPT, 0);

	outportw(IO_HBLANK_TIMER, 1);
	outportw(IO_TIMER_CTRL, HBLANK_TIMER_ENABLE | HBLANK_TIMER_REPEAT);

	// enable wonderswan vblank interrupt
	ws_hwint_enable(HWINT_VBLANK | HWINT_HBLANK_TIMER);

	vblank_fired = 0x01;

	// enable cpu interrupts
	cpu_irq_enable();
}

void main(void)
{
	disable_interrupts();

	ws_mode_set(WS_MODE_COLOR_4BPP);

	// disable all video output for now
	outportw(IO_DISPLAY_CTRL, 0);

	// set base addresses for screens 1 and 2
	outportb(IO_SCR_BASE, SCR1_BASE(screen_1) | SCR2_BASE(screen_2));

	// set sprite base address
	outportb(IO_SPR_BASE, SPR_BASE(sprites));

	// reset scroll registers to 0
	outportb(IO_SCR1_SCRL_X, 16);
	outportb(IO_SCR1_SCRL_Y, 0);
	outportb(IO_SCR2_SCRL_X, 0);
	outportb(IO_SCR2_SCRL_Y, 0);
	
	// don't render any sprites for now
	outportb(IO_SPR_COUNT, 0);

	// load palettes
	ws_dma_copy_words(MEM_COLOR_PALETTE(0), gfx_wave_palette, gfx_wave_palette_size);

	// load gfx
	ws_dma_copy_words(MEM_TILE_4BPP(0), gfx_wave_tiles, gfx_wave_tiles_size);

	// load tilemap
	ws_dma_copy_words(screen_1, gfx_wave_map, gfx_wave_map_size);

	// enable just screen_1
	outportw(IO_DISPLAY_CTRL, DISPLAY_SCR1_ENABLE);

	tic = 0;
	palette_tic = 0;
	scroll_tic = 0;

	enable_interrupts();

	while(1)
	{
		while (!vblank_fired)
		{
			cpu_halt();
		}

		vblank_fired = 0x00;
	}
}
