/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */

#ifndef LENA_TEST_PF_FF_MAC_SCHEDULER_H
#define LENA_TEST_PF_FF_MAC_SCHEDULER_H

#include "ns3/simulator.h"
#include "ns3/test.h"


using namespace ns3;


/**
* This system test program creates different test cases with a single eNB and 
* several UEs, all having the same Radio Bearer specification. In each test 
* case, the UEs see the same SINR from the eNB; different test cases are 
* implemented obtained by using different SINR values and different numbers of 
* UEs. The test consists on checking that the obtained throughput performance 
* is equal among users is consistent with the definition of proportional 
* fair scheduling
*/
class LenaPfFfMacSchedulerTestCase1 : public TestCase
{
  public:
    LenaPfFfMacSchedulerTestCase1 (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRef);
    virtual ~LenaPfFfMacSchedulerTestCase1 ();

  private:
    virtual void DoRun (void);
    uint16_t m_nUser;
    uint16_t m_nLc;
    uint16_t m_dist;
    double m_thrRef;
};


class LenaPfFfMacSchedulerTestCase2 : public TestCase
{
  public:
    LenaPfFfMacSchedulerTestCase2 (uint16_t nUser, std::vector<uint16_t> dist, std::vector<uint16_t> estThrPf);
    virtual ~LenaPfFfMacSchedulerTestCase2 ();
    
  private:
    virtual void DoRun (void);
    uint16_t m_nUser;
    std::vector<uint16_t> m_dist;
    std::vector<uint16_t> m_estThrPf;
};




class LenaTestPfFfMacSchedulerSuite : public TestSuite
{
  public:
    LenaTestPfFfMacSchedulerSuite ();
};




#endif /* LENA_TEST_PF_FF_MAC_SCHEDULER_H */