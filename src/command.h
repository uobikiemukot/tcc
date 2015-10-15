/* macro command definition:
 *
 * buttons:
 *   S: start
 *   1: button1
 *   2: button2
 *   3: button3
 *   4: button4
 *   5: button5
 *   6: button6
 *
 * button layout:
 *   button1(K) button2(S) button3(HS)
 *   button4(P) button5(-) button6(D)
 *
 * only press/release
 *   _n: hold n button
 *   ^n: release n button
 *
 * directions:
 *   U: upper
 *   L: left
 *   R: right
 *   D: down
 *   RD: lower right
 *   LD: lower left
 *   RU: upper right
 *   LU: upper left
 *
 * wait:
 *   Wn: wait n frame
 *
 * others:
 *   .: 1 frame advance
 *   ,: simultaneous input
 *
 */

enum command_misc_t {
	NBUTTON     = 11,
	FRAME_LIMIT = 128,
};

enum button_t {
	BUTTON_UNKNOWN = -1,
	BUTTON0        = 0, /* 0: not used */
	BUTTON1        = 1,
	BUTTON2        = 2,
	BUTTON3        = 3,
	BUTTON4        = 4,
	BUTTON5        = 5,
	BUTTON6        = 6,
	BUTTON7        = 7,
	BUTTON8        = 8,
	BUTTON_SELECT  = 9,
	BUTTON_START   = 10,
};

enum button_state_t {
	BUTTON_NONE     = -1,
	BUTTON_RELEASED = 0,
	BUTTON_PRESSED  = 1,
};

enum hat_angle_t {
	HAT_NONE        = -2,
	HAT_NEUTRAL     = -1,
	HAT_UP          = 0,
	HAT_UPPER_RIGHT = 45,
	HAT_RIGHT       = 90,
	HAT_LOWER_RIGHT = 135,
	HAT_DOWN        = 180,
	HAT_LOWER_LEFT  = 225,
	HAT_LEFT        = 270,
	HAT_UPPER_LEFT  = 315,
};

enum axis_t {
	AXIS_NONE    = -1,
	AXIS_NEUTRAL = 512,
	AXIS_LOW     = 0,
	AXIS_HIGH    = 1023,
};

enum command_type_t {
	COMMAND_UNKNOWN  = -1, 
	COMMAND_BUTTON    = 0,
	COMMAND_DIRECTION = 1,
	COMMAND_WAIT      = 2,
	COMMAND_OTHERS    = 3,
};

struct command_state_t {
	enum button_state_t button[NBUTTON];
	//enum hat_angle_t hat;
	enum axis_t x, y;
	int wait;
};

enum button_t char2button(const char c)
{
	switch (c) {
	case '1':
		return BUTTON1;
	case '2':
		return BUTTON2;
	case '3':
		return BUTTON3;
	case '4':
		return BUTTON4;
	case '5':
		return BUTTON5;
	case '6':
		return BUTTON6;
	case '7':
		return BUTTON7;
	case '8':
		return BUTTON8;
	case 'S':
		return BUTTON_START;
	default:
		return BUTTON_UNKNOWN;
	};
}

enum hat_angle_t str2angle(const char str[2])
{
	switch (str[0]) {
	case 'N':
		return HAT_NEUTRAL;
	case 'U':
		return HAT_UP;
	case 'D':
		return HAT_DOWN;
	case 'R':
		return
			(str[1] == 'U') ? HAT_UPPER_RIGHT:
			(str[1] == 'D') ? HAT_LOWER_RIGHT:
			HAT_RIGHT;
	case 'L':
		return
			(str[1] == 'U') ? HAT_UPPER_LEFT:
			(str[1] == 'D') ? HAT_LOWER_LEFT:
			HAT_LEFT;
	default:
		return HAT_NEUTRAL;
	}
}

void str2axis(const char str[2], enum axis_t *x, enum axis_t *y)
{
	switch (str[0]) {
	case 'N':
		*x = AXIS_NEUTRAL;
		*y = AXIS_NEUTRAL;
	case 'U':
		*x = AXIS_NEUTRAL;
		*y = AXIS_LOW;
		break;;
	case 'D':
		*x = AXIS_NEUTRAL;
		*y = AXIS_HIGH;
		break;;
	case 'R':
		if (str[1] == 'U') {
			*x = AXIS_HIGH;
			*y = AXIS_LOW;
		} else if (str[1] == 'D') {
			*x = AXIS_HIGH;
			*y = AXIS_HIGH;
		}  else {
			*x = AXIS_HIGH;
			*y = AXIS_NEUTRAL;
		}
		break;;
	case 'L':
		if (str[1] == 'U') {
			*x = AXIS_LOW;
			*y = AXIS_LOW;
		} else if (str[1] == 'D') {
			*x = AXIS_LOW;
			*y = AXIS_HIGH;
		}  else {
			*x = AXIS_LOW;
			*y = AXIS_NEUTRAL;
		}
		break;;
	default:
		*x = AXIS_NONE;
		*y = AXIS_NONE;
		break;
	}
}

void wait_nframe(int n)
{
	double msec = (double) n * (1.0 / 60) * 1000;

#ifdef TEST_MODE
	printf("wait_nframe:%lf (%d frame)\n", msec, n);
#else
	delay(msec);
#endif
}

enum command_type_t check_command_type(char c)
{
	switch (c) {
	case 'S':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return COMMAND_BUTTON;
	case 'U':
	case 'L':
	case 'R':
	case 'D':
	case 'N':
		return COMMAND_DIRECTION;
	case 'W':
		return COMMAND_WAIT;
	case '.':
	case ',':
	case '_':
	case '^':
		return COMMAND_OTHERS;
	default:
		return COMMAND_UNKNOWN;
	}
}

int is_valid_char(int c)
{
	enum command_type_t type = check_command_type(c);

	if (type == COMMAND_UNKNOWN)
		return false;
	else
		return true;
}

int check_hold(char *command, bool *press_only, bool *release_only)
{
	if (command[0] == '_') {
		*press_only = true;
		return 1;
	} else if (command[0] == '^') {
		*release_only = true;
		return 1;
	} else {
		return 0;
	}
}

int command_button(char *command, struct command_state_t *frame, int findex)
{
	bool press_only = false, release_only = false;
	enum button_t bindex;
	char *cp;

	cp = command + check_hold(command, &press_only, &release_only);
	bindex = char2button(*cp);

	if (release_only == false) {
		frame[findex].button[bindex]     = BUTTON_PRESSED;
		frame[findex].wait               = 1;
	}

	if (press_only == false) {
		frame[findex + 1].button[bindex] = BUTTON_RELEASED;
		frame[findex + 1].wait           = 1;
	}

	return 1;
}

int command_direction(char *command, struct command_state_t *frame, int findex)
{
	enum axis_t x, y;
	bool press_only = false, release_only = false;
	char *cp;

	cp = command + check_hold(command, &press_only, &release_only);
	//str2axis(cp, &frame[findex].x, &frame[findex].y);
	str2axis(cp, &x, &y);

	if (release_only == false) {
		frame[findex].x = x;
		frame[findex].y = y;
		frame[findex].wait = 1;
	}

	if (press_only == false) {
		frame[findex + 1].x = AXIS_NEUTRAL;
		frame[findex + 1].y = AXIS_NEUTRAL;
		frame[findex + 1].wait = 1;
	}

	return 1;
}

int command_wait(char *command, struct command_state_t *frame, int findex)
{
	if (findex > 0)
		frame[findex - 1].wait = strtol(command + 1, NULL, 10);
	return 0;
}

int command_others(char *command, struct command_state_t *frame, int findex)
{
	int finc = 0;
	enum command_type_t type;

	if (*command == '_' || *command == '^')  {
		type = check_command_type(*(command + 1));

		if (type == COMMAND_BUTTON)
			finc = command_button(command, frame, findex);
		else if (type == COMMAND_DIRECTION)
			finc = command_direction(command, frame, findex);
	}

	return finc;
}

int (*command_func[])(char *command, struct command_state_t *frame, int findex) = {
	[COMMAND_BUTTON]    = command_button,
	[COMMAND_DIRECTION] = command_direction,
	[COMMAND_WAIT]      = command_wait,
	[COMMAND_OTHERS]    = command_others,
	//[COMMAND_UNKNOWN]   = NULL,
};

void command_send(char *command)
{
	int findex = 0;
	struct command_state_t frame[FRAME_LIMIT];
	struct parm_t parm, inner_parm;
	enum command_type_t type;

	for (int i = 0; i < FRAME_LIMIT; i++) {
		for (int b = 0; b < NBUTTON; b++)
			frame[i].button[b] = BUTTON_NONE;

		//frame[i].hat = HAT_NONE;
		frame[i].x = AXIS_NONE;
		frame[i].y = AXIS_NONE;

		frame[i].wait = -1;
	}
#ifndef TEST_MODE
	Joystick.hat(HAT_NEUTRAL);
	Joystick.X(AXIS_NEUTRAL);
	Joystick.Y(AXIS_NEUTRAL);
#endif

	reset_parm(&parm);
	parse_arg(command, &parm, '.', is_valid_char);

	for (int i = 0; i < parm.argc && parm.argv[i] != NULL; i++) {
		reset_parm(&inner_parm);
		parse_arg(parm.argv[i], &inner_parm, ',', is_valid_char);

		for (int j = 0; j < inner_parm.argc; j++) {
			type = check_command_type(*inner_parm.argv[j]);

			if (type != COMMAND_UNKNOWN && command_func[type] != NULL)
				command_func[type](inner_parm.argv[j], frame, findex);
		}
		findex++;
	}

	for (int i = 0; i <= findex; i++) {

		/*
		if (frame[i].hat != HAT_NONE)
#ifdef TEST_MODE
			printf("Joystick.hat(%d)\n", frame[i].hat);
#else
			Joystick.hat(frame[i].hat);
#endif
		*/

		if (frame[i].x != AXIS_NONE || frame[i].y != AXIS_NONE) {
#ifdef TEST_MODE
			printf("Joystick.X(%d)\n", frame[i].x);
			printf("Joystick.Y(%d)\n", frame[i].y);
#else
			Joystick.X(frame[i].x);
			Joystick.Y(frame[i].y);
#endif
		}

		for (int b = 0; b < NBUTTON; b++) {
			if (frame[i].button[b] != BUTTON_NONE) {
#ifdef TEST_MODE
				printf("Joystick.button(%d, %d)\n", b, frame[i].button[b]);
#else
				Joystick.button(b, frame[i].button[b]);
#endif
			}
		}

		if (frame[i].wait > 0)
			wait_nframe(frame[i].wait);
	}
}
