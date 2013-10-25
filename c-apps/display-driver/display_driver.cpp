#include <unistd.h>
#include <string.h>
#include <iostream>

#include "display_driver.h"

// static inits
bool display_driver::terminate = false;
display_driver * display_driver::instance_p = NULL;

display_driver::display_driver()
{
	mover_p = new spi_mover("/dev/spidev0.0");

	for(int i = 0; i < NUM_STRINGS; i++)
	{
		strings[i] = new string_cb(CHARS_PER_STRING);
	}
	
	pthread_mutex_init(&data_mutex, NULL);
	
	// spawn a thread
	display_driver::terminate = false;
	pthread_create(&worker_thread, // thread structure
					NULL, 			// attributes
					&thread_func,	// body
					NULL);			// argv
					
	instance_p = this;
}

display_driver::~display_driver()
{

	display_driver::terminate = true;
	
	pthread_join(worker_thread, NULL);

	delete mover_p;
}

		
bool display_driver::update_string(int index, char * new_string)
{
	bool retval = true;

	pthread_mutex_lock(&data_mutex);
	
	if(index > NUM_STRINGS)
	{
		std::cerr << "Display Error: Invalid string index requested: " << index << std::endl;
		goto cleanup;
	}
	
	strings[index]->replace_string(new_string);
	
cleanup:	
	pthread_mutex_unlock(&data_mutex);
	
	return retval;
}
		
void* display_driver::thread_func(void * X)
{
	char c = 0;
	
	while(1)
	{
		if(display_driver::terminate)
		{
			std::cout << "thread exiting" << std::endl;
			pthread_exit(0);
		}
	
		if(instance_p != NULL)
		{
			instance_p->thread_member_func(c);
		}
			
		usleep(500000);
		//sleep(1);
		//std::cout << "." << std::endl;
	}
}

void display_driver::thread_member_func(int i)
{
	uint8_t out[NUM_STRINGS * CHARS_PER_STRING * 2];
	
	// Only needed because the spi_mover only does bidirectional xfer...
	// if it had a plain "write" we might not need this.
	uint8_t in[NUM_STRINGS * CHARS_PER_STRING * 2];

#if 1
	pthread_mutex_lock(&data_mutex);

	// Render the string buffer
	for(int i = 0; i < NUM_STRINGS; i++)
	{
		out[0] = (font['X'] & 0xff00) >> 8;
		out[1] = font['X'] & 0xff;
	}
	
	// then write the buffer to the displays

	if( ! mover_p->transfer(2, out, in))
	{
		std::cerr << "Dsiplay transfer() error!" << std::endl;
	}
	
	
	pthread_mutex_unlock(&data_mutex);
#else
	uint8_t out[2], in[2];

	out[0] = (font[i] & 0xff00) >> 8;
	out[1] = font[i] & 0xff;

	mover_p->transfer(2, out, in);
	
	std::cout << "sending segment: " << i << std::endl;
#endif
}

const uint16_t display_driver::font[128] =
{
	// unprintable all as simply 0
	0x0000,// 0	000	00	00000000	NUL	&#000;	 	Null char
	0x0000,// 1	001	01	00000001	SOH	&#001;	 	Start of Heading
	0x0000,// 2	002	02	00000010	STX	&#002;	 	Start of Text
	0x0000,// 3	003	03	00000011	ETX	&#003;	 	End of Text
	0x0000,// 4	004	04	00000100	EOT	&#004;	 	End of Transmission
	0x0000,// 5	005	05	00000101	ENQ	&#005;	 	Enquiry
	0x0000,// 6	006	06	00000110	ACK	&#006;	 	Acknowledgment
	0x0000,// 7	007	07	00000111	BEL	&#007;	 	Bell
	0x0000,// 8	010	08	00001000	 BS	&#008;	 	Back Space
	0x0000,// 9	011	09	00001001	 HT	&#009;	 	Horizontal Tab
	0x0000,// 10	012	0A	00001010	 LF	&#010;	 	Line Feed
	0x0000,// 11	013	0B	00001011	 VT	&#011;	 	Vertical Tab
	0x0000,// 12	014	0C	00001100	 FF	&#012;	 	Form Feed
	0x0000,// 13	015	0D	00001101	 CR	&#013;	 	Carriage Return
	0x0000,// 14	016	0E	00001110	 SO	&#014;	 	Shift Out / X-On
	0x0000,// 15	017	0F	00001111	 SI	&#015;	 	Shift In / X-Off
	0x0000,// 16	020	10	00010000	DLE	&#016;	 	Data Line Escape
	0x0000,// 17	021	11	00010001	DC1	&#017;	 	Device Control 1 (oft. XON)
	0x0000,// 18	022	12	00010010	DC2	&#018;	 	Device Control 2
	0x0000,// 19	023	13	00010011	DC3	&#019;	 	Device Control 3 (oft. XOFF)
	0x0000,// 20	024	14	00010100	DC4	&#020;	 	Device Control 4
	0x0000,// 21	025	15	00010101	NAK	&#021;	 	Negative Acknowledgement
	0x0000,// 22	026	16	00010110	SYN	&#022;	 	Synchronous Idle
	0x0000,// 23	027	17	00010111	ETB	&#023;	 	End of Transmit Block
	0x0000,// 24	030	18	00011000	CAN	&#024;	 	Cancel
	0x0000,// 25	031	19	00011001	 EM	&#025;	 	End of Medium
	0x0000,// 26	032	1A	00011010	SUB	&#026;	 	Substitute
	0x0000,// 27	033	1B	00011011	ESC	&#027;	 	Escape
	0x0000,// 28	034	1C	00011100	 FS	&#028;	 	File Separator
	0x0000,// 29	035	1D	00011101	 GS	&#029;	 	Group Separator
	0x0000,// 30	036	1E	00011110	 RS	&#030;	 	Record Separator
	0x0000,// 31	037	1F	00011111	 US	&#031;	 	Unit Separator

	// Real chars start here
	0x0000,// 32	040	20	00100000	 	&#32;	 	Space
	0x0000,// 33	041	21	00100001	!	&#33;	 	Exclamation mark
	0x0000,// 34	042	22	00100010	"	&#34;	&quot;	Double quotes (or speech marks)
	0x0000,// 35	043	23	00100011	#	&#35;	 	Number
	0x0000,// 36	044	24	00100100	$	&#36;	 	Dollar
	0x0000,// 37	045	25	00100101	%	&#37;	 	Procenttecken
	0x0000,// 38	046	26	00100110	&	&#38;	&amp;	Ampersand
	0x0100,// 39	047	27	00100111	'	&#39;	 	Single quote
	0x00ac,// 40	050	28	00101000	(	&#40;	 	Open parenthesis (or open bracket)
	0xac00,// 41	051	29	00101001	)	&#41;	 	Close parenthesis (or close bracket)
	0x5353,// 42	052	2A	00101010	*	&#42;	 	Asterisk
	0x1111,// 43	053	2B	00101011	+	&#43;	 	Plus
	0x0000,// 44	054	2C	00101100	,	&#44;	 	Comma
	0x1010,// 45	055	2D	00101101	-	&#45;	 	Hyphen
	0x0000,// 46	056	2E	00101110	.	&#46;	 	Period, dot or full stop
	0x0202,// 47	057	2F	00101111	/	&#47;	 	Slash or divide
	
	// usable digits here
	0xaeae,// 48	060	30	00110000	0	&#48;	 	Zero
	0x0101,// 49	061	31	00110001	1	&#49;	 	One
	0x9c9c,// 50	062	32	00110010	2	&#50;	 	Two
	0xbc94,// 51	063	33	00110011	3	&#51;	 	Three
	0x3830,// 52	064	34	00110100	4	&#52;	 	Four
	0xb4b4,// 53	065	35	00110101	5	&#53;	 	Five
	0xb4bc,// 54	066	36	00110110	6	&#54;	 	Six
	0x0682,// 55	067	37	00110111	7	&#55;	 	Seven
	0xbcbc,// 56	070	38	00111000	8	&#56;	 	Eight
	0x3cb0,// 57	071	39	00111001	9	&#57;	 	Nine
	
	// more unusable...
	0x0000,// 58	072	3A	00111010	:	&#58;	 	Colon
	0x0000,// 59	073	3B	00111011	;	&#59;	 	Semicolon
	0x0000,// 60	074	3C	00111100	<	&#60;	&lt;	Less than (or open angled bracket)
	0x0000,// 61	075	3D	00111101	=	&#61;	 	Equals
	0x0000,// 62	076	3E	00111110	>	&#62;	&gt;	Greater than (or close angled bracket)
	0x0000,// 63	077	3F	00111111	?	&#63;	 	Question mark
	0x0000,// 64	100	40	01000000	@	&#64;	 	At symbol
	
	// upcase letters here
	0x3cb8,// 65	101	41	01000001	A	&#65;	 	Uppercase A
	0xb1bc,// 66	102	42	01000010	B	&#66;	 	Uppercase B
	0x84ac,// 67	103	43	01000011	C	&#67;	 	Uppercase C
	0xb81c,// 68	104	44	01000100	D	&#68;	 	Uppercase D
	0x94bc,// 69	105	45	01000101	E	&#69;	 	Uppercase E
	0x14b8,// 70	106	46	01000110	F	&#70;	 	Uppercase F
	0xb4ac,// 71	107	47	01000111	G	&#71;	 	Uppercase G
	0x3838,// 72	110	48	01001000	H	&#72;	 	Uppercase H
	0x8585,// 73	111	49	01001001	I	&#73;	 	Uppercase I
	0x0585,// 74	112	4A	01001010	J	&#74;	 	Uppercase J
	0x4301,// 75	113	4B	01001011	K	&#75;	 	Uppercase K
	0x802c,// 76	114	4C	01001100	L	&#76;	 	Uppercase L
	0x2a68,// 77	115	4D	01001101	M	&#77;	 	Uppercase M
	0x6868,// 78	116	4E	01001110	N	&#78;	 	Uppercase N
	0xacac,// 79	117	4F	01001111	O	&#79;	 	Uppercase O
	0x01cb8,// 80	120	50	01010000	P	&#80;	 	Uppercase P
	0xecac,// 81	121	51	01010001	Q	&#81;	 	Uppercase Q
	0x5cb8,// 82	122	52	01010010	R	&#82;	 	Uppercase R
	0xb4b4,// 83	123	53	01010011	S	&#83;	 	Uppercase S
	0x0581,// 84	124	54	01010100	T	&#84;	 	Uppercase T
	0xa82c,// 85	125	55	01010101	U	&#85;	 	Uppercase U
	0x022a,// 86	126	56	01010110	V	&#86;	 	Uppercase V
	0x682a,// 87	127	57	01010111	W	&#87;	 	Uppercase W
	0x4242,// 88	130	58	01011000	X	&#88;	 	Uppercase X
	0x0241,// 89	131	59	01011001	Y	&#89;	 	Uppercase Y
	0x8686,// 90	132	5A	01011010	Z	&#90;	 	Uppercase Z
	
	// more unused
	0x0000,// 91	133	5B	01011011	[	&#91;	 	Opening bracket
	0x0000,// 92	134	5C	01011100	\	&#92;	 	Backslash
	0x0000,// 93	135	5D	01011101	]	&#93;	 	Closing bracket
	0x0000,// 94	136	5E	01011110	^	&#94;	 	Caret - circumflex
	0x0000,// 95	137	5F	01011111	_	&#95;	 	Underscore
	0x0000,// 96	140	60	01100000	`	&#96;	 	Grave accent
	
	// recycle the uppercase here...
	0x3cb8,// 97	141	61	01100001	a	&#97;	 	Lowercase a
	0xb1bc,// 98	142	62	01100010	b	&#98;	 	Lowercase b
	0x84ac,// 99	143	63	01100011	c	&#99;	 	Lowercase c
	0xb81c,// 100	144	64	01100100	d	&#100;	 	Lowercase d
	0x94bc,// 101	145	65	01100101	e	&#101;	 	Lowercase e
	0x14b8,// 102	146	66	01100110	f	&#102;	 	Lowercase f
	0xb4ac,// 103	147	67	01100111	g	&#103;	 	Lowercase g
	0x3838,// 104	150	68	01101000	h	&#104;	 	Lowercase h
	0x8585,// 105	151	69	01101001	i	&#105;	 	Lowercase i
	0x0585,// 106	152	6A	01101010	j	&#106;	 	Lowercase j
	0x4301,// 107	153	6B	01101011	k	&#107;	 	Lowercase k
	0x802c,// 108	154	6C	01101100	l	&#108;	 	Lowercase l
	0x2a68,// 109	155	6D	01101101	m	&#109;	 	Lowercase m
	0x6868,// 110	156	6E	01101110	n	&#110;	 	Lowercase n
	0xacac,// 111	157	6F	01101111	o	&#111;	 	Lowercase o
	0x01cb,// 112	160	70	01110000	p	&#112;	 	Lowercase p
	0xecac,// 113	161	71	01110001	q	&#113;	 	Lowercase q
	0x5cb8,// 114	162	72	01110010	r	&#114;	 	Lowercase r
	0xb4b4,// 115	163	73	01110011	s	&#115;	 	Lowercase s
	0x0581,// 116	164	74	01110100	t	&#116;	 	Lowercase t
	0xa82c,// 117	165	75	01110101	u	&#117;	 	Lowercase u
	0x022a,// 118	166	76	01110110	v	&#118;	 	Lowercase v
	0x682a,// 119	167	77	01110111	w	&#119;	 	Lowercase w
	0x4242,// 120	170	78	01111000	x	&#120;	 	Lowercase x
	0x0241,// 121	171	79	01111001	y	&#121;	 	Lowercase y
	0x8686,// 122	172	7A	01111010	z	&#122;	 	Lowercase z
	
	// more unused
	0x0000,// 123	173	7B	01111011	{	&#123;	 	Opening brace
	0x0000,// 124	174	7C	01111100	|	&#124;	 	Vertical bar
	0x0000,// 125	175	7D	01111101	}	&#125;	 	Closing brace
	0x0000,// 126	176	7E	01111110	~	&#126;	 	Equivalency sign - tilde
	0x0000,// 127	177	7F	01111111		&#127;	 	Delete
};


