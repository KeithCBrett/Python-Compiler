@startuml
partition calc_CFG_loop_const {
    start
    fork
        :CFGNode *t_inp_out = INPUT;
    fork again
        :size_t t_inp_index = INPUT;
    fork end
    while ((t_inp_out != label) && (t_inp_out->left != t_inp_index))
        :t_inp_out = t_inp_out->next;
    endwhile
    :OUTPUT t_inp_out;
    stop
}
@enduml
