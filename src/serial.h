#ifndef SERIAL_H_
#define SERIAL_H_
#endif

static void set_baudrate(struct termios *opt, unsigned int baudrate) {
	cfsetispeed(opt, baudrate);
	cfsetospeed(opt, baudrate);
}

static void set_data_bit(struct termios *opt, unsigned int databit) {
	opt->c_cflag &= ~CSIZE;
	switch (databit) {
	case 8:
		opt->c_cflag |= CS8;
		break;
	case 7:
		opt->c_cflag |= CS7;
		break;
	case 6:
		opt->c_cflag |= CS6;
		break;
	case 5:
		opt->c_cflag |= CS5;
		break;
	default:
		opt->c_cflag |= CS8;
		break;
	}
}

static void set_parity(struct termios *opt, char parity) {
	switch (parity) {
	case 'N':
		/* 无校验 */
		/* 偶校验 */
		/* 奇校验 */
	case 'n':
		opt->c_cflag &= ~PARENB;
		break;
	case 'E':
	case 'e':
		opt->c_cflag |= PARENB;
		opt->c_cflag &= ~PARODD;
		break;
	case 'O':
	case 'o':
		opt->c_cflag |= PARENB;
		opt->c_cflag |= ~PARODD;
		break;
		/* 其它选择为无校验 */
	default:
		opt->c_cflag &= ~PARENB;
		break;
	}
}

static void set_stopbit(struct termios *opt, const char *stopbit) {
	if (0 == strcmp(stopbit, "1")) {
		opt->c_cflag &= ~CSTOPB;
		/* 1 位停止位 t */
		/* 1.5 位停止位 */
		/* 2 位停止位 */
		/* 1 位停止位 */
	} else if (0 == strcmp(stopbit, "1.5")) {
		opt->c_cflag &= ~CSTOPB;
	} else if (0 == strcmp(stopbit, "2")) {
		opt->c_cflag |= CSTOPB;
	} else {
		opt->c_cflag &= ~CSTOPB;
	}
}

static int set_port_attr(int fd, int baudrate, int databit, const char *stopbit,
		char parity, int vtime, int vmin) {
	struct termios opt;

	tcgetattr(fd, &opt);

	set_baudrate(&opt, baudrate);

	// enable the receiver and set local mode ...
	// opt.c_cflag |= CLOCAL | CREAD;

	// Hardware flow control off
	 opt.c_cflag &= ~CRTSCTS;
	// Hardware flow control on
	// opt.c_cflag |= CRTSCTS;

	/* | CRTSCTS */
	set_data_bit(&opt, databit);
	set_parity(&opt, parity);
	set_stopbit(&opt, stopbit);

	// Original, no need CR
	// opt.c_cflag |= ~(ICANON | ECHO | ECHOE | ISIG);
	// Classical, designed to line, need CR or LF
	// opt.c_cflag |= (ICANON | ECHO | ECHOE);

	opt.c_oflag = 0;
	opt.c_lflag |= 0;

	// Original output
	opt.c_oflag &= ~OPOST;
	// Classical output
	// opt.c_oflag |= OPOST;

	opt.c_cc[VTIME] = vtime;
	opt.c_cc[VMIN] = vmin;

	tcflush(fd, TCIFLUSH);

	return (tcsetattr(fd, TCSANOW, &opt));
}

