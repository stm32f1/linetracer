#ifndef CONVERSION_H
#define CONVERSION_H

#define ADC_to_V(adc)	((double)(adc) / 4095 * 3.3)
#define mm_to_step(l)   ((uint32_t)((double)(l) * 2.450))	//200 * 2 / 52π
#define step_to_mm(l)   ((double)((uint32_t)(l) / 2.450))	//200 * 2 / 52π
#define deg_to_step(d)  ((uint32_t)((double)(d) * 2.05))      //92 / 52 * 200*2 / 360 
#define step_to_deg(s)  ((uint16_t)((double)(s) / 2.05))	//1.966
//#define PSD_to_mm(psd)	(40.0+(2.2 - ADC_to_V(psd))/(2.2-0.55)*(500.0 - 40.0))
#define PSD_to_mm(psd)	(50.0+(2673.0-(double)psd)/(2673.0-2020.0)*(200.0-50.0))
#define mm_to_PSD(l)  (2673.0-((double)l-50)*(2673.0-2020.0)/(200.0-50.0))

#endif /* CONVERSION_H */
