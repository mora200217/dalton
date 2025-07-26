// Generic incremental quadrature decoder
// The module outputs the number of pulses every UPDATE_RATE clock cycles
// The velocity must be found with an external MCU as
// vel = p_cnt*resolution/(UPDATE_RATE*clk_period)
// We use the design proposed by Gorbounov (2015)
// https://journals.iugaza.edu.ps/index.php/JERT/article/download/1661/1593

module quadrature_decoder #(
    parameter UPDATE_RATE=12,   // rate of update of p_cnt (in # of clock cycles)
    parameter PPR=4,            // base resolution of the encoder
    parameter MAX_P_CNT=4*4*1   // enough to store ppr*4*#rev pulses
)(
    input clk,
    input rst,
    input a,
    input b,
    output reg [$clog2(PPR*4*2)-1:0] p_cnt
);

reg a_prev;
reg b_prev;
reg ap_prev;
reg bp_prev;

// We assume UPDATE_RATE*CLK_T is smaller than the time it takes the encoding wheel
// to make 2 revs. Since we detect the direction four times per each pulse,the 
// resolution is increased by 4
reg [$clog2(PPR*4*2)-1:0] r_p_cnt;

wire reset_dir;
assign reset_dir = ap & ap_prev || bp & bp_prev;
reg dir_err;

reg [$clog2(UPDATE_RATE)-1:0] clk_cnt;

// Metastability guards on asynchronous inputs
reg r_a, g_a;
reg r_b, g_b;
always @(posedge clk) begin
    g_a <= a;
    g_b <= b;

    r_a <= g_a;
    r_b <= g_b;
end

// Edge detectors
always @(posedge clk) begin
    a_prev <= r_a;
    b_prev <= r_b;

    if (f_quad) begin
        ap_prev <= ap;
        bp_prev <= bp;
    end
end

// Update direction in pos- and negedge of both a and b
wire ap, bp, f_quad;
assign ap = r_a ^ a_prev;
assign bp = r_b ^ b_prev;
assign f_quad = ap || bp;
always @(posedge clk) begin
    if (r_a ^ r_b) begin
        if (ap) dir_err <= 1'b1;
        else if (bp) dir_err <= 1'b0;
        else dir_err <= dir_err;
    end
    else dir_err <= dir_err;
end

// Correct errors due to direction change
wire dir;
assign dir = reset_dir ? ~dir_err : dir_err;

// Generate direction pulses
wire up, down;
assign up = f_quad & dir;
assign down = f_quad & ~dir;

// Update velocity every UPDATE_RATE clock cycles
always @(posedge clk, posedge rst) begin
    if (rst) begin
        r_p_cnt <= 0;
        p_cnt <= 0;
        clk_cnt <= 0;
    end
    else begin
        if (clk_cnt == UPDATE_RATE) begin
            p_cnt <= r_p_cnt;
            r_p_cnt <= 0;
            clk_cnt <= 0;
        end
        else begin
            clk_cnt <= clk_cnt + 1;
            if (f_quad) begin
                r_p_cnt <= up ? r_p_cnt + 1 : r_p_cnt - 1;
            end
        end
    end
end
endmodule

