@startuml
partition calc_SSA_nth_predecessor {
    start
    fork
        :size_t t_inp_index <- INPUT;
    fork again
        :Size_tLL t_inp_list <- INPUT;
    fork end
    :size_t t_c <- 0;
    while (t_c != t_inp_index)
        :t_c <- t_c + 1;
        :t_inp_list <- t_inp_list.next;
    endwhile
    :OUTPUT t_inp_list.contents;
    stop
}
@enduml
