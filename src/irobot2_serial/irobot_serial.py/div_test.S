.global div, div_hw

    .text
div_test:
    li a0, 32767
    li a1, 32767
    call div_hw
    #call div

