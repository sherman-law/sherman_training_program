#include "gpio.h"
#include "timer.h"
#include "systick.h"
#include "scheduler.h"
/*
 * \brief    Scheduler Class Implementation
 *
 */
typedef struct _scheduler{
	fp_get_micros	        get_micros;	        /* get time in micros */	
	fp_innerloop 	        inloop;		        /* inner loop */
	fp_sched_run	        run;		        /* scheduling algorithm */
	fp_count                count;              /* inner loop runing count */  
	uint16_t               _loop_rate;	        /* inner loop rate */
	uint32_t               _loop_count; 	    /* inner loop counter */
	uint8_t                _run_flag;           /* inner loop run enable flag */
	uint32_t               _ticks;		        /* timer ticks */
	uint8_t	               _task_num;	        /* task numbers according to user defined task array */
	task_t	              *_array;		        /* user defined task array */
}sched_t;

sched_t g_sched;                /* create the scheduler */

/*
 * \brief Global Scheduler Handle
 * 
 * \note  This global scheduler handle holds the only instance of the scheduler during program running.
 * 	  It is used in case a scheduler pointer could not be passed as a context of a function call.
 */
sched_t *__g_sched = &g_sched;	/* pointed to global sched obj */


/*
 * forward declaration
 */
uint32_t default_get_micros (void);
void default_innerloop(sched_t *thiz, void* ctx);
void default_run(sched_t *thiz, uint32_t time_available);
static void count(sched_t *thiz);
static inline int32_t _get_loop_period_us(sched_t *thiz);	
static inline int32_t _get_loop_period_ms(sched_t *thiz);

/**
 * \brief  default scheduler constructor 
 *
 * \note   user can override this function to initialize their own inner loop
 * 	   and other scheduler properties.
 */
extern uint32_t default_get_micros (void);
void default_innerloop(sched_t *thiz, void* ctx);

extern void default_init(sched_t ** psched, task_t *task_array, uint8_t task_num)
{
	thiz->get_micros	      = default_get_micros;       
	thiz->inloop 	              = default_innerloop;
	thiz->run		      = default_run;
	thiz->count                   = count;
	thiz->_loop_rate	      = 200;
	thiz->_loop_count 	      = 0; 
	thiz->_run_flag 	      = 0; 
	thiz->_ticks	 	      = 0; 
	thiz->_task_num 	      = task_num;
	thiz->_array		      = task_array;
}

/*
 * \brief  The main scheduling algorithm default implementation
 * 
 * \param[in]  thiz: this pointer points to the scheduler itself
 * \param[in]  time_available: remaining time after running the inner_loop
 *
 * \note       user can override this function by providing their own scheduling algorithm implementations. 
 * 
 */
void default_run (sched_t *thiz, uint32_t time_available)
{
	uint32_t task_time_allowed 	= 0;
	uint32_t task_time_started 	= 0;
	uint32_t task_time_taken	= 0;
	uint8_t  current_task 		= 0;
	task_t	*cur_task		= NULL;
	
	uint32_t now = thiz->get_micros();
	uint8_t i = 0;
	
	for (i = 0; i < thiz->_task_num; i++) {
		current_task = i;
		cur_task = thiz->_array + current_task;
		uint16_t dt = thiz->_loop_count - cur_task->_last_ticks;
	       	uint16_t interval_ticks = thiz->_loop_rate / cur_task->_rate_hz;
		
        	if (interval_ticks < 1) {
            		interval_ticks = 1;
        	}
	
	    if (dt >= interval_ticks) {
            	// this task is due to run. Do we have enough time to run it?
            	task_time_allowed = cur_task->_max_time;

            	if (dt >= interval_ticks * 2) {
		//TODO
                // we've slipped a whole run of this task!
                //printf some information about task;
				
            	}
		if (task_time_allowed <= time_available) {
                    // run it
                    task_time_started = now;
                    cur_task->run(cur_task, NULL);

                    // record the tick counter when we ran. This drives
                    // when we next run the event
                    cur_task->_last_ticks = thiz->_loop_count;

                    // work out how long the event actually took
                    now = thiz->get_micros();
                    task_time_taken = now - task_time_started;

                    if (task_time_taken > task_time_allowed) {
		    //TODO
                    // the event overran!
                    //printf error
                    }
				
                    if (task_time_taken >= time_available) {
			thiz->_run_flag = 1;
			return;
                    }
                    time_available -= task_time_taken;
                }
            }
        }
		
}
/**
 * \brief  inner_loop function which should be placed into main loop
 *
 * \note   this is a default implementation of inner loop
 */
void default_innerloop (sched_t *thiz, void* ctx)
{
	ctx = NULL; // supress compiler warning, will be optimized out
	uint32_t l_count = thiz->_loop_count;
	uint32_t l_count_m = (l_count >> 16) << 16; 
	if ( l_count == l_count_m )
		debug("------ inner loop running --------\n");

}

/**
 * \brief  main_loop function which should be placed into while(1)
 *
 */
void main_loop (sched_t *thiz)
{
	_RETURN_IF_FAIL(thiz);
	uint32_t start_time       = 0;
	uint32_t end_time         = 0;
	uint16_t time_available	  = 0;

	if (thiz->_run_flag == 1) {	        						/* Wait for the 5ms mark bit */
		thiz->_run_flag = 0;

		start_time = thiz->get_micros();
		thiz->inloop(thiz, NULL);       	 					/* execute the inner loop */
		end_time = thiz->get_micros();  

		thiz->count(thiz);             							/* inner loop counts +1 */
		
		time_available = ( _get_loop_period_us(thiz) + start_time ) - end_time;    /* calculate the remaining time */
		
		thiz->run(thiz, time_available);        					/* start scheduling with time available */
	}
}

/*
 * \brief set the method scheduler uses to get time values (in micros )
 *
 */
void set_get_micros(sched_t *thiz, fp_get_micros fp)
{
	_RETURN_IF_FAIL(thiz);
	thiz->get_micros = fp;	
}

/**
 * \brief  timer callback, used to decide if it is time for inner loop to run
 *
 * \note   this function should be called from a 1ms timer's ISR
 *
 */
void timer_tick_callback (void)
{
	_RETURN_IF_FAIL(thiz);
	if (( thiz->_ticks++ % _get_loop_period_ms(thiz)) == 0) {
		thiz->_run_flag = 1;
	}
}

/**
 * \brief default scheduler destructor
 *
 */
void default_destroy(sched_t *thiz)
{
	_RETURN_IF_FAIL(thiz);
	thiz->get_micros = NULL;
	thiz->inloop = NULL;
	thiz->run = NULL;
	thiz->_loop_rate = 0;
	thiz->_loop_count = 0;
	thiz->_run_flag = 0;
	thiz->_ticks = 0;
	thiz->_millis = 0;
	thiz->_task_num = 0;
	thiz->_task_array = NULL;
	_SAFE_FREE(thiz);
}

/**
 * \brief  inner_loop running count
 *
 */
void count(sched_t *thiz)
{
	thiz->_loop_count++;
}

/**
 * \brief  get inner loop running rate
 *
 * \retval  inner loop running rate
 */
static inline int16_t _get_loop_rate_hz(sched_t *thiz) 
{
	_RETURN_MINUS_IF_FAIL(thiz);
	return thiz->_loop_rate;
}

/**
 * \brief  get Inner Loop period in micro seconds 
 *
 * \param[in]  thiz:this pointer
 *
 * \retval  Inner Loop period in micro seconds
 */
static inline int32_t _get_loop_period_us(sched_t *thiz) 
{
	_RETURN_MINUS_IF_FAIL(thiz);
	// TODO: better idea to avoid integer division?
	return (int32_t)1000000UL / thiz->_loop_rate;
}

/**
 * \brief  get Inner Loop period in milli seconds 
 *
 * \param[in]  thiz:this pointer
 *
 * \retval  Inner Loop period in milli seconds
 */
static inline int32_t _get_loop_period_ms(sched_t *thiz) 
{
	_RETURN_MINUS_IF_FAIL(thiz);
	// TODO: better idea to avoid integer division?
	return (int32_t)1000UL / thiz->_loop_rate;
}

