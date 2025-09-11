@startuml
partition calc_SSA_append_to_pred_list {
    start
    fork
        :PredLL *t_inp_list <- INPUT;
    fork again
        :size_t t_inp_element <- INPUT;
    fork end
    :PredLL *t_head <- t_inp_list;
    if (t_head = NULL) then (TRUE)
        :t_head <- malloc(sizeof(t_head));
        :t_head.next <- NULL;
        :t_head.contents <- t_inp_element;
    else (FALSE)
        while (t_inp_list.next != NULL)
            :t_inp_list <- t_inp_list.next;
        endwhile
        :t_inp_list.next <- t_inp_element;
    endif
    :OUTPUT t_head;
    stop
}
@enduml
