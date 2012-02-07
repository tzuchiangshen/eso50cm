else if( goto_flag == 2 ) {
                    if( encPos < trgPos ) {
      	                trgDir = 1;
      	                diffPos = trgPos - encPos;
      	            } else {
          	            trgDir = 0;
          	            diffPos = encPos - trgPos;
          	        }
                    trgThreshold = 25;
                    if( diffPos < 50 )  {
                        trgEncVelRef = 0;
                        goto_flag = 0;
                        output_high( rLED );
                	} else if( diffPos < 19200 ) {
                    	trgEncVelRef  = diffPos;
                    	trgEncVelRef *= 0.25;
                    	sec = p_tmr0rv;
              	        goto_flag = 3;
                  	} else {
                      	trgEncVelRef  = 9600;
                    	sec = p_tmr0rv;
              	        goto_flag = 3;
                    }
                                
                  	if( trgDir > 0 ) {
                  	    encVelRef = trgEncVelRef;
                  	} else {
                        encVelRef = -trgEncVelRef;
                    }
                    trgDirOld = trgDir;
                    m_MtrCtrl = NEWPID;
                }
  	        }
  	        
  	        
  	        
  	        
  	        
  	        
    
        
            
                
                    
                        
                            
                                            } else if( goto_flag == 3 ) {
                sec --;
                if( trgDir ==  1 ) {
                    if( encPos + trgThreshold > trgPos ) {
                        encVelRef = 0;
                        m_MtrCtrl = NEWPID;
                        goto_flag = 2;
                    } else {
                        if( sec == 0 ) {
                            goto_flag = 2;
                            sec = p_tmr0rv;
          	                output_toggle( rLED );
                        }                        
                    }
                } else {
                    if( encPos < trgPos + trgThreshold  ) {
                        encVelRef = 0;
                        m_MtrCtrl = NEWPID;
                        goto_flag = 2;
                    } else {
                        if( sec == 0 ) {
                            goto_flag = 2;
                            sec = p_tmr0rv;
          	                output_toggle( rLED );
                        }                        
                    }
                }
            } else if( goto_flag == 4 ) {
                if( encVel == 0 ) {
                    goto_flag = 1;
                    h_interr  = 0;
                }
            } else if( goto_flag == 5 ) {
                sec --;

                } else {
                    if( encPos < trgPos + trgThreshold ) {
                        p_Vfin = 0;
                        m_MtrCtrl = RAMPA;
                        //encVelRef = 0;
                        //m_MtrCtrl = NEWPID;
                        goto_flag = 4;
                    } else {
                        if( sec == 0 ) {
                            goto_flag = 1;
                            sec = p_tmr0rv;
  	                        output_toggle( rLED );
  	                    }
  	                }
                }
            }
              	  
