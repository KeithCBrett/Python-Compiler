@startuml
partition calc_CFG_instr {
    start
    fork
        :VasmInstr *t_inp_vasm = INPUT;
    fork again
        :size_t t_inp_index = INPUT;
    fork end
    fork
        :t_out = *t_inp_vasm;
    fork again
        :t_c = 0;
    fork end
    while (t_c != t_inp_index)
        fork
            :t_out = t_out->next;
        fork again
            :t_c++;
        fork end
    endwhile
    :OUTPUT t_out;
    stop
}
@enduml
