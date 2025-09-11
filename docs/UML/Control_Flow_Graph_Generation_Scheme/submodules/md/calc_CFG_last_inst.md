@startuml
partition calc_CFG_last_inst {
    start
    :VasmInst *t_inp_vasm = INPUT;
    :t_out = t_inp_vasm;
    while (t_out->next != NULL)
        :t_out = t_out->next;
    endwhile
    :OUTPUT t_out;
    stop
}
@enduml
