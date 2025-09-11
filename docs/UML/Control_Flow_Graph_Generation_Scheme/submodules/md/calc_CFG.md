@startuml
partition calc_CFG {
    start
    :VasmInst *inp_vasm = INPUT;
    fork
        :s = calc_CFG_num_blocks(inp_vasm);
    fork again
        :h = calc_CFG_head_arr(inp_vasm);
    fork again
        :t = calc_CFG_tail_arr(inp_vasm);
    fork again
        :c = 0;
    fork end
    :out[s];
    while (c < s)
        fork
            :xh = calc_CFG_inst(h[c], inp_vasm);
        fork again
            :xt = calc_CFG_inst(t[c], inp_vasm);
        fork end
        :out[c] = calc_CFG_blocks(xh, xt);
        :c++;
    endwhile
    :out = calc_CFG_edges(out);
    :OUTPUT out;
    stop
}
@enduml
