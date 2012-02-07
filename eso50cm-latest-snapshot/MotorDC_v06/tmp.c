
            if( traking_flag ) {
                trgPos += 60;   
            }
            if( encPos < trgPos ) {
  	            trgDir = 1;
  	            diffPos = trgPos - encPos;
  	        } else {
      	        trgDir = 0;
      	        diffPos = encPos - trgPos;
      	    }
      	    if( goto_flag ) {
          	    if( trgEncVelRef < 1200 ) {
              	    if( diffPos < 100 ) {
                        goto_flag = false;
                        encVelRef = 0;
                        m_MtrCtrl = PID;
                    }
                }
            }
      	    
   	        sec --;
            if( sec == 0 ) {
  	            sec = p_tmr0rv;
  	            output_toggle( rLED );
	        
	            
    	        if( goto_flag ) {
        	        if( diffPos < 2400 ) {
            	        trgEncVelRef = 600;
                    } else if( diffPos < 4800 ) {
            	        trgEncVelRef = 1200
            	    } else {
                	    if( encVel == 0 ) {
                	        trgEncVelRef = 600;
                	    } else {
                	        else if( diffPos < 9600 ) {
            	                trgEncVelRef = 2400;
                	        } if( diffPos < 19200 ) {
            	                trgEncVelRef = 4800;
                	        } else if( diffPos < 38400 ) {
                	            trgEncVelRef = 9600;
                	        } else if( diffPos < 76800 ) {
                	            trgEncVelRef = 19200;
                	        } else if( diffPos < 153600 ) {
                	            trgEncVelRef = 38400;
                	        } else if( diffPos < 307200 ) {
                	            trgEncVelRef = 76800;
                	        } else {
                	            trgEncVelRef = 76800;
                	        }
                	    }
                	}
            	    if( trgDir > 0 ) {
            	        encVelRef = trgEncVelRef;
            	    } else {
                	    encVelRef = -trgEncVelRef;
                	}
            	    m_MtrCtrl = PID;
        	    }
        	    
            }
