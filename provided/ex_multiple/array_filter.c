int max_element_index(int * t, int len) {
    int max = t[0];
    int max_index = 0;
    int i = 0;
    while (i < len) {
        if (t[i] > max) {
            max = t[i];
            max_index = i;
        }
        i++;
    }
    return max_index;
}
