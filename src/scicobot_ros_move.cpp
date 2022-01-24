/***********************************************************************************
 *  @file       scicobot_ros_move.cpp
 *  Project     scicobot_hardware
 *  @brief      *****
 *
 *  @author     Otávio Augusto Rocha da Cruz
 *  @bug 		 No known bugs.
 *  License     MIT
 *
 *  @section License
 *
 * Copyright (c) 2021 SciCoBot
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**********************************************************************************/
#include "scicobot_ros_move.h"
#include <led_debug.h>

void ScicobotRosMove::initSubscriberMove(ScicobotRos* scicobotRos, char* topicName)
{   
	#if SCICOBOT_ROS_MOVE_BEST_EFFORT == 0
	DEBUG_ERROR_MICR0_ROS(rclc_subscription_init_default(
		&subscriberMove,
		scicobotRos->get_rcl_node(),
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
		topicName));
	#else
		DEBUG_ERROR_MICR0_ROS(rclc_subscription_init_best_effort(
		&subscriberMove,
		scicobotRos->get_rcl_node(),
		ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
		topicName));
	#endif
	
	#if SCICOBOT_ROS_MOVE_DEBUG == 1
		debugObj.DEBUGLN("initSubscriberMove() sucess"); 
	#endif
}

void ScicobotRosMove::initExecutorMove(ScicobotRos* scicobotRos, int numberHandles)
{
	
	DEBUG_ERROR_MICR0_ROS(rclc_executor_init(&executorMove, 
		&(*scicobotRos->get_rclc_support()).context, 
		numberHandles, 
		scicobotRos->get_rcl_allocator()));
	
	#if SCICOBOT_ROS_MOVE_DEBUG == 1
		debugObj.DEBUGLN("initExecutorMove() sucess"); 
	#endif
}

void ScicobotRosMove::addExecutorsMove(void (*subscriberMoveCallback)(const void *))
{
	DEBUG_ERROR_MICR0_ROS(rclc_executor_add_subscription(&executorMove,
		&subscriberMove, &msgMove, subscriberMoveCallback, ON_NEW_DATA));
	
	#if SCICOBOT_ROS_MOVE_DEBUG == 1
		debugObj.DEBUGLN("addExecutorsMove() sucess"); 
	#endif
}

void ScicobotRosMove::initRosMoveSubscriber(ScicobotRos* scicobotRos, void (*subscriberMoveCallback)(const void *))
{
	this->initSubscriberMove(scicobotRos, "/move_on");
	
	this->initExecutorMove(scicobotRos, 1);
	
	this->addExecutorsMove(subscriberMoveCallback);

	#if SCICOBOT_ROS_MOVE_DEBUG == 1
		debugObj.DEBUGLN("Topic create. Executor init. Callback added to executor."); 
	#endif
}

rclc_executor_t* ScicobotRosMove::get_rclc_executor()
{
	#if SCICOBOT_ROS_MOVE_DEBUG == 1
		debugObj.DEBUG("Position of executorMove is: "); 
		debugObj.DEBUGLN(&executorMove); 
	#endif
	
	return &executorMove;
}





