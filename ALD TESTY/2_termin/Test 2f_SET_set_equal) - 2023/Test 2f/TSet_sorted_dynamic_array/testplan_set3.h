{
TEST_MESSAGE("Vlozeni elementu: 14, 6")
TEST_OPERATION(set_insert)
TEST_INPUTS(14, 6)
TEST_OUTPUT( 1, 1)
//TEST_LIST_OPERATIONS
//TEST_REPORT_BEFORE
TEST_REPORT_AFTER
TEST_PRINT_MEMORY_STATS
},

{
TEST_MESSAGE("Vlozeni elementu: 4")
TEST_OPERATION(set_insert)
TEST_INPUTS(4,)
TEST_OUTPUT(1,)
//TEST_LIST_OPERATIONS
//TEST_REPORT_BEFORE
TEST_REPORT_AFTER
TEST_PRINT_MEMORY_STATS
},

{
TEST_MESSAGE("Vlozeni elementu: 12, 2, 10, 0, 8")
TEST_OPERATION(set_insert)
TEST_INPUTS(12, 2, 10, 0, 8)
TEST_OUTPUT( 1, 1,  1, 1, 1)
//TEST_LIST_OPERATIONS
//TEST_REPORT_BEFORE
TEST_REPORT_AFTER
TEST_PRINT_MEMORY_STATS
},

{
TEST_MESSAGE("Test pritomnosti elementu: 0, 2, 4, 6, 8, 10, 12, 14")
TEST_OPERATION(set_is_element_of)
TEST_INPUTS(0, 2, 4, 6, 8, 10, 12, 14)
TEST_OUTPUT(1, 1, 1, 1, 1,  1,  1,  1)
//TEST_LIST_OPERATIONS
//TEST_REPORT_BEFORE
//TEST_REPORT_AFTER
},

{
TEST_MESSAGE("Test odebrani neexistujiciho elementu: 11")
TEST_OPERATION(set_erase)
TEST_INPUTS(11)
TEST_OUTPUT(0)
//TEST_LIST_OPERATIONS
//TEST_REPORT_BEFORE
//TEST_REPORT_AFTER
//TEST_PRINT_MEMORY_STATS
},

{
TEST_MESSAGE("Odebrani elementu: 0, 2, 4, 6, 8, 10, 12, 14")
TEST_OPERATION(set_erase)
TEST_INPUTS(0, 2, 4, 6, 8, 10, 12, 14)
TEST_OUTPUT(1, 1, 1, 1, 1,  1,  1,  1)
//TEST_LIST_OPERATIONS
//TEST_REPORT_BEFORE
//TEST_REPORT_AFTER
TEST_PRINT_MEMORY_STATS
},
