@startuml
partition calc_SSA_dominator_intersect {
    start
    fork
        :size_t t_inp_b1 <- INPUT;
    fork again
        :size_t t_inp_b2 <- INPUT;
    fork again
        :size_t *t_inp_doms <- INPUT;
    fork end
    fork
        :size_t t_pointer_left <- t_inp_b1;
    fork again
        :size_t t_pointer_right <- t_inp_b2;
    fork end
    while (t_pointer_left != t_pointer_right)
        while (t_pointer_left > t_pointer_right)
            :t_pointer_left <- t_inp_doms[t_pointer_left];
        endwhile
        while (t_pointer_right > t_pointer_left)
            :t_pointer_right <- t_inp_doms[t_pointer_right];
        endwhile
    endwhile
    :OUTPUT t_pointer_left;
    stop
}
@enduml
