@startuml
partition calc_CFG_block {
    start
    fork
        :VasmInst t_head = INPUT;
    fork again
        :VasmInst t_tail = INPUT;
    fork end
    fork
        :VasmInst *t_out = &t_head;
    fork again
        :VasmInst *t_temp = &t_head;
    fork end
    while (t_temp != t_tail)
        :t_temp = t_temp->next;
    endwhile
    :t_temp->next = NULL;
    :OUTPUT t_out;
    stop
}
@enduml
