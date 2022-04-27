namespace scale{
	extern long weight;
	extern long weight_tare;
	extern long weight_display;
	extern long weight_display_tare;
	extern long filtered_raw_value;
	extern long tare_value;

	void init();
	void update();
	bool is_ready();
	void wait_ready();
	long read_raw();
	void tare();
}
