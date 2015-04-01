#include <stdio.h>
#include "mailbox/mailbox.h"
#include "mailbox/xmailbox.h"

static void print_general_info(unsigned p[], int fd);
static void print_fb_info(unsigned p[], int fd);

int main()
{
	int fd = xmbox_open();
	unsigned p[0x10000];

	print_general_info(p, fd);
	print_fb_info(p, fd);

	xmbox_close(fd);

	return 0;
}

void print_general_info(unsigned p[], int fd)
{
	mb_get_firmware_revision(p, fd);
	printf("firmware_revision: 0x%08x\n", p[5]);

	mb_get_board_model(p, fd);
	printf("board_model: 0x%08x\n", p[5]);

	mb_get_board_revision(p, fd);
	printf("board_revision: 0x%08x\n", p[5]);

	mb_get_board_mac_address(p, fd);
	printf("board_mac_address: %02x:%02x:%02x:%02x:%02x:%02x\n", p[5] & 0xff, (p[5] >> 8) & 0xff, (p[5] >> 16) & 0xff, (p[5] >> 24) & 0xff, p[6] & 0xff, (p[6] >> 8) & 0xff);

	mb_get_board_serial(p, fd);
	printf("board_serial: 0x%08x%08x\n", p[6], p[5]);

	mb_get_arm_memory(p, fd);
	printf("arm memory: addr=0x%08x size=0x%08x\n", p[5], p[6]);

	mb_get_vc_memory(p, fd);
	printf("vc memory: addr=0x%08x size=0x%08x\n", p[5], p[6]);
}

void print_fb_info(unsigned p[], int fd)
{
	int i;

	mbfb_initialize(p, &i);

	mbfb_append_get_physical_width_height(p, &i);
	mbfb_append_get_virtual_width_height(p, &i);
	mbfb_append_get_depth(p, &i);
	mbfb_append_get_pixel_order(p, &i);
	mbfb_append_get_alpha_mode(p, &i);
	mbfb_append_get_pitch(p, &i);
	mbfb_append_get_virtual_offset(p, &i);
	mbfb_append_get_overscan(p, &i);
	mbfb_append_get_palette(p, &i);

	mbfb_finalize(p, &i);
	mbfb_do(p, fd);

	printf("physical_width_height: width=%d height=%d\n", p[5], p[6]);

	printf("virtual_width_height: width=%d height=%d\n", p[10], p[11]);

	printf("depth: %d\n", p[15]);

	printf("pixel_order: ");
	switch (p[19]) {
		case 0:
			printf("RGB");
			break;
		case 1:
			printf("RGB");
			break;
		default:
			printf("(error)");
	}
	printf("\n");

	printf("alpha_mode: ");
	switch (p[23]) {
		case 0:
			printf("enabled");
			break;
		case 1:
			printf("reversed");
			break;
		case 2:
			printf("ignored");
			break;
		default:
			printf("(error)");
	}
	printf("\n");

	printf("pitch: %d\n", p[27]);

	printf("virtual offset: X=%d Y=%d\n", p[31], p[32]);

	printf("overscan: top=%d bottom=%d left=%d right=%d\n", p[36], p[37], p[38], p[39]);

	printf("palette: ");
	for (i = 0; i < 1024 / (int) sizeof(p[0]); i++)
		printf("0x%08x%c", p[43 + i], i == 1024 / (int) sizeof(p[0]) - 1 ? '\n' : ' ');
}
