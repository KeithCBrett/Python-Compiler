@startuml
partition calc_CFG_tail_arr {
    start
    :VasmInstr *t_inp_vasm = INPUT;
    fork
        :t_h = calc_CFG_head_arr(t_inp_vasm);
    fork again
        :t_c = 0;
    fork again
        :t_s = calc_CFG_num_blocks(t_inp_vasm);
    fork end
        :t_out[t_s];
    while (t_c != (t_s-1))
        :t_out[t_c] = t_h[t_c+1]) - 1;
        :t_c++;
    endwhile
    :t_out[t_s] = calc_CFG_num_list_items(t_inp_vasm);
    :OUTPUT t_out;
    stop
}
@enduml
