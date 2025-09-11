@startuml
partition calc_CFG_head_arr {
    start
    :VasmInstr t_inp_vasm = INPUT;
    fork
        :t_s = calc_CFG_num_blocks(t_inp_vasm);
    fork again
        :t_c = 0;
    fork end
    :t_out[t_s];
    while (t_inp_vasm != NULL)
        if (t_inp_vasm is head of list) then (TRUE)
            :t_out = t_c;
            fork
                :t_out++;
            fork again
                :t_inp_vasm = t_inp_vasm->next;
            fork end
        elseif (t_inp_vasm->type == label) then (TRUE)
            :t_out = t_c;
            fork
                :t_out++;
            fork again
                :t_inp_vasm = t_inp_vasm->next;
            fork end
        elseif (t_inp_vasm->type == jle) then (TRUE)
            fork
                :t_inp_vasm = t_inp_vasm->next;
            fork again
                :t_c++;
            fork end
            :t_out = t_c;
            :t_out++;
        else (FALSE)
        endif
    endwhile
    :OUTPUT t_out;
    stop
}
@enduml
