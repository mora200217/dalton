TARGET = quadrature_decoder_tb.vvp
sim: *.v
	iverilog -g2012 $^ -o $(TARGET)
	vvp $(TARGET)

clean:
	rm *.vvp *.vcd

