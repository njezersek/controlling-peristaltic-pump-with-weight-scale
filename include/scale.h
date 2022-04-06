namespace scale{
	extern long weight;
	extern long weight_display;
	extern long filtered_raw_value;

	extern long offset;
	extern float factor;

	void init();
	void update();
	bool is_ready();
	void wait_ready();
	long read_raw();
}
