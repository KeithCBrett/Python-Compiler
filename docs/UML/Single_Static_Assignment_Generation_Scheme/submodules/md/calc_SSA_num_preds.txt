@startuml
partition calc_SSA_num_preds {
    fork
        :Size_tLL *t_inp_pred_arr <- INPUT;
    fork again
        :size_t t_inp_index <- INPUT;
    fork end
    fork
        :Size_tLL t_list <- t_inp_pred_arr[t_inp_index];
    fork again
        :size_t t_c <- 1;
    fork end
    while (t_list.next != NULL)
        :t_list <- t_list.next;
        :t_c <- t_c + 1;
    endwhile
    :OUTPUT t_c;
}
@enduml
