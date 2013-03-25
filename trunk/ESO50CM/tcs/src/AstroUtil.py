import math

def degs2HHMMSS(degs):  
    m_degs = math.fabs(degs)
    gg  = int(math.floor(m_degs))
    tmp = float(gg)
    mm = int(math.floor(( m_degs - tmp ) * 60.0))
    tmp += float(mm) / 60.0
    ss = int(math.floor((m_degs - tmp ) * 3600.0))
    tmp += float(ss) / 3600.0
    cc   = int(round(( m_degs - tmp ) * 360000. ))

    if( cc > 99 ):
        cc = 0
        ss = ss + 1
    if( ss > 59  ):
        ss = 0
        mm = mm + 1
    
    if( mm > 59 ):
        mm = 0
        gg = gg + 1
    
    if( degs  < 0 ):
        gg = gg * -1 
    
    format = [gg,mm,ss]
    return format

def HHMMSS2degs(hh=0, mm=0, ss=0.0):
    degs  = math.fabs(hh) + (mm/60.0) + (ss/3600.0)
    degs  = degs * 15.0
    if( hh < 0 ):
        degs = degs * -1.0
    return degs

def sexagesimal2degs(hhmmss, isRA=True):
    tmp = hhmmss.split(":")
    if(len(tmp) != 3): 
        return -1
    if (hhmmss[0] == '-'):
        #print "negative offset"
        degs = -1.0 * math.fabs(float(tmp[0]) + float(tmp[1])/60.0 + float(tmp[2])/3600.0)
    else: 
        degs = math.fabs(float(tmp[0]) + float(tmp[1])/60.0 + float(tmp[2])/3600.0)

    if(isRA):
        degs  = degs * 15.0
    #print "%s:%s:%s = %f degs" % (tmp[0], tmp[1], tmp[2], degs) 
    return degs

def DDMMSS2degs(dd=0, mm=0, ss=0):
    degs  = math.fabs(dd) + (mm/60.0) + (ss/3600.0)
    if( dd < 0 ):
        degs = degs * -1.0
    return degs
        
