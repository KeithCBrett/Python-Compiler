@startuml
partition calc_SSA_append_to_df {
    start
    fork
        :size_t t_inp_element <- INPUT;
    fork again
        :DominanceFrontier t_inp_df <- INPUT;
    fork end
    fork
        :DominanceFrontier *t_head <- &t_inp_df;
    fork again
        :bool duplicate <- false;
    fork end
    if (t_inp_df.contents = t_inp_element) then (TRUE)
        :duplicate <- true;
    endif
    while (t_inp_df.next != NULL)
        :t_inp_df <- t_inp_df.next;
        if (t_inp_df.contents = t_inp_element) then (TRUE)
            :duplicate <- true;
        endif
    endwhile
    if (duplicate = false) then (TRUE)
        :t_inp_df.next <- t_inp_element;
    end if
    :OUTPUT t_head;
    stop
}
@enduml
