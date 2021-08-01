int compute_soundringnow() {
int check_spot=-1;
//fprintf(stderr,"b4	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
if (soundringsend == -1) { // not playing - just a zero
  if (soundringnow != -1) {
    if (soundringlast ==-1) {
      soundringnow = -1; // soundringlast not set means we should not be in here
//	fprintf(stderr,"nnol	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
      return soundringnow;
      }
    if (soundringnow == soundringlast) { // we are on the last one
      soundringnow = -1; // we must have just changed this;
      soundringlast = -1;  // but invalidate it next time.
//	fprintf(stderr,"nw=ls	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
      return soundringnow;
      }
    soundringnow = (soundringnow+1) % SOUNDRING_COUNT;
    if (soundringnow == soundringlast) { // we are on the last one
      soundringnow = -1; // we must have just changed this;
      soundringlast = -1;  // but invalidate it next time.
//	fprintf(stderr,"nw=ls2	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
      return soundringnow;
      }
    if (soundringnow == soundringhead) { // ok we bumped into the head
      soundringnow = -1;
//	fprintf(stderr,"nw=hd	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
      return soundringnow;
      }
    return soundringnow;
    }  //if soundringnow was set - we simply bump it up one -- within range.
  else { // soundringnow is -1 and soundringsend is -1
    if (soundringlast == -1) {
      if (soundringhead != -1) {
        check_spot = soundringhead;
	}
      else {
        soundringnow = -1;
//	fprintf(stderr,"hd-1	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
        return soundringnow;
	} 
      }
    else {
      check_spot = soundringlast;
      }
    }
  } // if soundringsend is not set
else {
  check_spot = soundringsend;
  }
if (check_spot==-1) {
  soundringnow = -1;
  soundringlast = -1;
//	fprintf(stderr,"cs-1	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
  return soundringnow;
  } // should be set - 
  
int back_size;
if (soundringfirst <0) {
  soundringnow = -1; // nothing started
  soundringlast = -1;
//	fprintf(stderr,"sf-1	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
  return soundringnow;
  }
back_size = check_spot - soundringfirst;
if (back_size<0) back_size += SOUNDRING_COUNT;
if (back_size >=the_sound_delay) {
  soundringnow = check_spot - the_sound_delay;	
  if (soundringnow <0) soundringnow += SOUNDRING_COUNT;
//	fprintf(stderr,"gd	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
  return soundringnow;
  }
else {
  soundringnow = -1;
  /* if we had a psuedo and actual soundring last, we could eprfect this, let me thingk about it - this is way complezx
      first     head  current tail 
            ^now
      no first
        no now
      
      first       last head tail
             ^ now
      now?   first l2st head tail
        next time
       now?  first  l2st head tail
        now? first  l2st head tail
	 ...
	   first now?    last      head   l2st tail
	   
	 yep need the l2st that goes up one till last is taken care of.
	 But what if head gets big?  well, we skip some.
	 yuck - there is no clean way
	 fuck l2st - if last is set and we are in range then grt else
	 just give up until we start again.
	 */
  if (soundringlast != -1) { // clear the soundringlast if we arent even up to the first buffer.  we will waituntil we get a buffer.
    soundringfirst = -1;
    soundringlast = -1;    
    }
//	fprintf(stderr,"early	cs %d	srs %d	srn %d	srf %d	srl %d	srh %d	srt %d\n",check_spot,soundringsend,soundringnow,soundringfirst,soundringlast,
//    soundringhead,soundringtail);
  return soundringnow;
  }
}  
      
      
