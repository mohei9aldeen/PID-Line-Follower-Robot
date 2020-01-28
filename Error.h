
/*      the weight of the sensors :
      right  5   2  0  -2   -5   left
*/
//                                                       BIN                                 
#define OFF_LEFT                                       0b00001    
#define REAL_LEFT                                      0b00011           
#define LEFT_STRAIGHT_INTERSECTION                     0b00111            
#define LEFT_NEAR_CENTER                               0b00110        

#define CENTER                                         0b00100    
    
#define RIGHT_NEAR_CENTER                              0b01100         
#define RIGHT_STRAIGHT_INTERSECTION                    0b11100       
#define REAL_RIGHT                                     0b11000     
#define OFF_RIGHT                                      0b10000                        


// intersections ....
#define T_INTERSECTION                                 0b11011     
#define CROSS_INTERSECTION                             0b11111      
#define NO_LINE                                        0b00000


/* define the  error values depending on the positions*/

#define OFF_RIGHT_ERROR                                5
#define REAL_RIGHT_ERROR                               3.5
#define RIGHT_STRAIGHT_INTERSECTION_ERROR              7/3
#define RIGHTT_NEAR_CENTER_ERROR                       1

#define CENTER_ERROR                                   0

#define LEFT_NEAR_CENTER_ERROR                         -1
#define LEFT_STRAIGHT_INTERSECTION_ERROR               -7/3
#define REAL_LEFT_ERROR                                -3.5
#define OFF_LEFT_ERROR                                 -5


#define T_INTERSECTION_ERROR                           -3.5
#define CROSS_INTERSECTION_ERROR                       0

/* end of the error values depending on the positions*/



