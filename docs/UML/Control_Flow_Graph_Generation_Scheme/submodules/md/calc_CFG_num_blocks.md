@startuml
partition calc_CFG_num_blocks {
    start
    :VasmInstr *t_inp_vasm = INPUT;
    :t_out = 0;
    while (t_inp_vasm != NULL)
        if (t_inp_vasm is head of list) then (TRUE)
            fork
                :t_out++;
            fork again
                :t_inp_vasm = t_inp_vasm->next;
            fork end
        elseif (t_inp_vasm->type == label) then (TRUE)
            fork
                :t_out++;
            fork again
                :t_inp_vasm = t_inp_vasm->next;
            fork end
        elseif (t_inp_vasm->type == jle) then (TRUE)
            fork
                :t_out++;
            fork again
                :t_inp_vasm = t_inp_vasm->next;
            fork end
        else (FALSE)
            :t_inp_vasm = t_inp_vasm->next;
        endif
    endwhile
    :OUTPUT t_out;
    stop
}
@enduml
