/*
 * $HEADER$
 */

/** @file **/

#include "lam_config.h"

#include "lam/constants.h"
#include "lam/runtime/runtime.h"
#include "lam/util/output.h"
#include "lam/threads/mutex.h"
#include "mca/lam/pcm/base/base.h"
#include "mca/lam/oob/base/base.h"
#include "mca/lam/registry/base/base.h"


/**
 * Initialze and setup a process in the LAM RTE.
 *
 * @retval LAM_SUCCESS Upon success.
 * @retval LAM_ERROR Upon failure.
 *
 * This function performs 
 */
int lam_rte_init(bool *allow_multi_user_threads, bool *have_hidden_threads)
{
  int ret;
  bool user_threads, hidden_threads;

  *allow_multi_user_threads = false;
  *have_hidden_threads = false;

  /* Added by JMS -- feel free to move around */

  if (LAM_SUCCESS != (ret = mca_pcm_base_open())) {
    /* JMS show_help */
    return ret;
  }
  if (LAM_SUCCESS != (ret = mca_pcm_base_select(&user_threads, 
                                                &hidden_threads))) {
    /* JMS show_help */
    return ret;
  }
  *allow_multi_user_threads |= user_threads;
  *have_hidden_threads |= hidden_threads;

  if (LAM_SUCCESS != (ret = mca_oob_base_open())) {
    /* JMS show_help */
    return ret;
  }
  if (LAM_SUCCESS != (ret = mca_oob_base_select(&user_threads, 
                                                &hidden_threads))) {
    /* JMS show_help */
    return ret;
  }
  *allow_multi_user_threads |= user_threads;
  *have_hidden_threads |= hidden_threads;

  if (LAM_SUCCESS != (ret = mca_registry_base_open())) {
    /* JMS show_help */
    return ret;
  }
  if (LAM_SUCCESS != (ret = mca_registry_base_select(&user_threads, 
                                                     &hidden_threads))) {
    /* JMS show_help */
    return ret;
  }
  *allow_multi_user_threads |= user_threads;
  *have_hidden_threads |= hidden_threads;

#if 0
  /*
   * BWB - this comment should be removed at some point in the very
   * near future
   *
   * JMS - will need more #include files to make this work.  Not
   * filling them in at the moment.  :-)
   *
   * This #if 0'ed out block of code is a rough approximation of what
   * should happen to get this parallel job bootstrapped and ready to
   * run.  There are probably some bugs in the OOB and PCM interfaces
   * that are going to make this really interesting (sorry :( ), but I
   * think it should work once the MPI modules are written...
   */

  /* Do the "right" MCA query and init functions to fire up the
   * run-time environment interfaces.  I'm not exactly sure what these
   * calls will be (since they are in the base functions, right?), but
   * do them here
   *
   * Order is:
   *   1) PCM
   *   2) OOB
   *   3) Registery
   *
   * Don't forget to close down in the reverse order at end of the day
   * - even the silly COFS implementations are going to leak resources
   * like crazy if you don't.
   *
   * The OOB system may not actually be usable until the end of
   * pcm_proc_startup, but must be initialized here.
   */

  /* Do the client side of the rendezvous with our launcher (or
   * whatever is needed for our RTE to figure out how to talk with our
   * peers and all that.
   */
  ret = mca_pcm.pcm_proc_startup();
  if (ret != MPI_SUCCESS) printf("oops!\n");

  /* at this point, we can use the OOB interface directly if we really
     need to, but is a bit tricky since we don't have a peers list
     yet. */
  mca_pcm.get_peers(&procs, &nprocs);

  /* get a pointer to me */
  my_proc = mca_pcm.get_me();

  /* get my parents.  need to think about how to do this - i don't
   *  think this is what we want at all...  We can probably ignore
   *  this for a little while since we don't have a run time
   *  environment tha supports spawn just yet, but something to
   *  remember...
   */
  mca_pcm.get_parent(&pprocs, &npprocs);

  /* we should have enough information by now to start running the PML
   * and PTL interfaces, right?
   */
#endif

  /* All done */

  return LAM_SUCCESS;
}
