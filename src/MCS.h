#ifndef _MCS_H
#define _MCS_H

#include "../config.h"

#include <ctime>
#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "MCSCompound.h"
#include "MCSMap.h"

namespace FMCS {

    class MCS {
    public:

        enum MatchType { DEFAULT, AROMATICITY_SENSETIVE, RING_SENSETIVE };

        enum RunningMode { FAST, DETAIL };
        

        MCS(const MCSCompound& compoundOne, const MCSCompound& compoundTwo,
            size_t userDefinedLowerBound, size_t substructureNumLimit,
            size_t atomMishmatchLower, size_t atomMismatchUpper,
            size_t bondMismatchLower, size_t bondMismatchUpper,
            MatchType mtype, RunningMode runningMode, int timeout);
        
        ~MCS() {}
        double getTime() {
            return timeUsed;
        }
        
        void calculate();
        
        size_t size() const { 
            if (identicalGraph) {
                return compoundOne.size();
            } else if (runningMode == FAST)
                return bestSize;
            else {
					 if(bestList.size()==0)
						 return 0;
                return bestList.front().size(); 
            }
        }
        
        const MCSCompound& getCompoundOne() const {
            if (haveBeenSwapped) {
                return compoundTwo; 
            } else {
                return compoundOne;
            }
        }
        const MCSCompound& getCompoundTwo() const { 
            if (haveBeenSwapped) {
                return compoundOne; 
            } else {
                return compoundTwo;
            }
        }

#ifdef HAVE_LIBOPENBABEL
        const std::list<std::string>& getFirstSmiResultStringList() const { 
            if (haveBeenSwapped) {
                return smiSet2; 
            } else {
                return smiSet1;
            }
        }
        
        const std::list<std::string>& getSecondSmiResultStringList() const { 
            if (haveBeenSwapped) {
                return smiSet1; 
            } else {
                return smiSet2;
            }
        }
#endif
        
        const std::list<std::string>& getFirstSdfResultStringList() const { 
            if (haveBeenSwapped) {
                return sdfSet2; 
            } else {
                return sdfSet1;
            }
        }
        
        const std::list<std::string>& getSecondSdfResultStringList() const { 
            if (haveBeenSwapped) {
                return sdfSet1; 
            } else {
                return sdfSet2;
            } 
        }
        
        const std::list<std::vector<size_t> >& getFirstOriginalIndice() const { 
            if (haveBeenSwapped) {
                return originalIdArray2; 
            } else {
                return originalIdArray1;
            }  
        }
        
        const std::list<std::vector<size_t> >& getSecondOriginalIndice() const { 
            if (haveBeenSwapped) {
                return originalIdArray1; 
            } else {
                return originalIdArray2;
            } 
        }
        
        void clearResult();
        
        bool isTimeout() const {
            return _isTimeout;
        }
        
    private:
        
        const MCSCompound& compoundOne;
        const MCSCompound& compoundTwo;

        size_t userDefinedLowerBound;
        size_t substructureNumLimit;

        size_t atomMismatchLowerBound;
        size_t atomMismatchUpperBound;

        size_t bondMismatchLowerBound;
        size_t bondMismatchUpperBound;

        MatchType matchType;

        RunningMode runningMode;

        int _timeout;

        size_t atomMismatchCurr;
        size_t bondMismatchCurr;

        size_t currSubstructureNum;

        double timeUsed;
		  clock_t startTime;
        size_t bestSize;
        bool identicalGraph;
        bool _isTimeout;
        bool haveBeenSwapped;


        

        std::list<MCSMap> bestList;

        MCSMap currentMapping;


        
        
        
        
        bool strictRingMatch;
        

        std::map<int, std::map<int, bool> > rules;
#ifdef HAVE_LIBOPENBABEL
        std::list<std::string> smiSet1;
        std::list<std::string> smiSet2;
#endif
        std::list<std::string> sdfSet1;
        std::list<std::string> sdfSet2;
        
        std::list<std::vector<size_t> > originalIdArray1;
        std::list<std::vector<size_t> > originalIdArray2;

        MCS(const MCS&);
        MCS& operator=(const MCS&);
        
        void max();
        
        bool compatible(size_t v1, size_t v2, size_t& bondMis, 
                        bool& introduceNewComponent) const;
        
        size_t top(MCSList<size_t>& v1_list);
        void boundary();
        void grow(MCSList<size_t>& v1_list, MCSList<size_t>& v2_list);
    };
    
}
#endif // _MCS_H
