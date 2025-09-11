@startuml
partition calc_CFG_num_list_items {
    start
    fork
        :VasmInstr *inp_vasm = OUTPUT;
    fork again
        :out = 0;
    fork end
    while (inp_vasm->next != NULL)
        fork
            :out++;
        fork again
            :inp_vasm = inp_vasm->next;
        fork end
    endwhile
    :OUTPUT out;
    stop
}
@enduml
