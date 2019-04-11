/*========================================================================
** ring_test.c
** Circular buffer testing
** ECEN5813
**========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "ring.h"

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

/*global variable declarations*/
ring_t *ring = NULL;
char data = 0x02;

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
   if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
      return -1;
   }
   else {
      return 0;
   }
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
   if (0 != fclose(temp_file)) {
      return -1;
   }
   else {
      temp_file = NULL;
      return 0;
   }
}

/* Test all functions with uninitialized ring buffer for failure
*  Test init function returns a valid pointer address 
*  Test resize function returns a valid pointer address
*/
void test_init(void)
{
    int ret = 0;
    /* test null pointers for each test */
    ret = insert(ring, data);
    CU_ASSERT_EQUAL(-1,ret);
    ret = r_remove(ring, &data);
    CU_ASSERT_EQUAL(-1,ret);
    ret = entries(ring);
    CU_ASSERT_EQUAL(-1,ret);

    init(3);
    /* verify ring initialized */
    CU_ASSERT_PTR_NOT_NULL(ring);
    /* verify buffer pointer initialized */
    CU_ASSERT_PTR_NOT_NULL(ring->Buffer);

    resize(10);
    /* verify ring initialized */
    CU_ASSERT_PTR_NOT_NULL(ring);
    /* verify buffer pointer initialized */
    CU_ASSERT_PTR_NOT_NULL(ring->Buffer);
    /* Verify length changed to 10 */
    CU_ASSERT(ring->Length == 10);
    
}

void test_insert(void)
{
    int ret = 0;
    /* check for failure return code */
    ret = insert(ring, data);
    CU_ASSERT_FALSE(ret);
    /* verify that data copied into buffer */
    CU_ASSERT(ring->Buffer[ring->Ini - 1] == data);
}

void test_remove(void)
{
    int ret = 0;
    char datalcl = 0;
    /* check for failure return code */
    ret = r_remove(ring, &datalcl);
    CU_ASSERT_FALSE(ret);
    /* verify data copied from buffer */
    CU_ASSERT(datalcl == data);
     
}

/* Test cases: insert to fill the buffer should not return any error
* the last insert (outside of the for loop) should return a -1 for a full buffer
* remove all information in the buffer should not return any errors
* the last remove (outside of the for loop) should return a -1 for no data in the buffer
*/
void edge_cases(void) 
{
    for (int i = 0; i < 9; i++)
    {
	CU_ASSERT_FALSE(insert(ring, data));
    }
    CU_ASSERT(insert(ring, data) == -1);

    for (int i = 0; i < 9; i++)
    {
	CU_ASSERT_FALSE(r_remove(ring, &data));
    }
    CU_ASSERT(r_remove(ring, &data) == -1);

}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test of init()", test_init)) ||
       (NULL == CU_add_test(pSuite, "test of insert()", test_insert)) ||
       (NULL == CU_add_test(pSuite, "test of r_remove()", test_remove)) ||
       (NULL == CU_add_test(pSuite, "test edge cases", edge_cases)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

