/**
 * \file
 *
 * \brief USB Host Driver header file for UOTGHS.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef UOTGHS_HOST_H_INCLUDED
#define UOTGHS_HOST_H_INCLUDED

#include "compiler.h"
#include "preprocessor.h"
#include "uotghs_otg.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

//! \ingroup usb_host_group
//! \defgroup uhd_group USB Host Driver (UHD)
//! UOTGHS low-level driver for USB host mode
//!
//! @{

//! @name UOTGHS Host IP properties
//!
//! @{
//! Get maximal number of endpoints
#define uhd_get_pipe_max_nbr()                (9)
#define UOTGHS_EPT_NUM                        (uhd_get_pipe_max_nbr()+1)
  //! Get maximal number of banks of endpoints
#define uhd_get_pipe_bank_max_nbr(ep)         ((ep == 0) ? 1 : (( ep <= 2) ? 3 : 2))
  //! Get maximal size of endpoint (3X, 1024/64)
#define uhd_get_pipe_size_max(ep)             (((ep) == 0) ? 64 : 1024)
  //! Get DMA support of endpoints
#define Is_uhd_pipe_dma_supported(ep)         ((((ep) >= 1) && ((ep) <= 6)) ? true : false)
  //! Get High Band Width support of endpoints
#define Is_uhd_pipe_high_bw_supported(ep)     (((ep) >= 2) ? true : false)
//! @}

//! @name Host Vbus line control
//!
//! VBOF is an optional output pin which allows to enable or disable
//! the external VBus generator.
//!
//! @{
//! Enables hardware control of USB_VBOF output pin when a Vbus error occur
#define uhd_enable_vbus_error_hw_control()    (Clr_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBUSHWC))
//! Disables hardware control of USB_VBOF output pin when a Vbus error occur
#define uhd_disable_vbus_error_hw_control()   (Set_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBUSHWC))
//! Set USB VBOF output pin polarity
#define uhd_set_vbof_active_high()            (Set_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBUSPO))
#define uhd_set_vbof_active_low()             (Clr_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBUSPO))
//! Requests VBus activation
#define uhd_enable_vbus()                     (UOTGHS->UOTGHS_SFR = UOTGHS_SR_VBUSRQ)
//! Requests VBus deactivation
#define uhd_disable_vbus()                    (UOTGHS->UOTGHS_SCR = UOTGHS_SR_VBUSRQ)
//! Tests if VBus activation has been requested
#define Is_uhd_vbus_enabled()                 (Tst_bits(UOTGHS->UOTGHS_SR, UOTGHS_SR_VBUSRQ))
//! @}

//! @name Host Vbus line monitoring
//!
//! The VBus level is always checked by UOTGHS hardware.
//!
//! @{
#define uhd_enable_vbus_error_interrupt()     (Set_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBERRE))
#define uhd_disable_vbus_error_interrupt()    (Clr_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBERRE))
#define Is_uhd_vbus_error_interrupt_enabled() (Tst_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBERRE))
#define uhd_ack_vbus_error_interrupt()        (UOTGHS->UOTGHS_SCR = UOTGHS_SCR_VBERRIC)
#define Is_uhd_vbus_error_interrupt()         (Tst_bits(UOTGHS->UOTGHS_SR, UOTGHS_SR_VBERRI))
//! @}

#define uhd_enable_errors_interrupt()         (Set_bits(UOTGHS->UOTGHS_CTRL, (UOTGHS_SCR_VBERRIC|UOTGHS_SCR_BCERRIC|UOTGHS_SCR_HNPERRIC|UOTGHS_SCR_STOIC)))
#define uhd_ack_errors_interrupt()            (UOTGHS->UOTGHS_SCR = (UOTGHS_SCR_VBERRIC|UOTGHS_SCR_BCERRIC|UOTGHS_SCR_HNPERRIC|UOTGHS_SCR_STOIC))
#define Is_uhd_errors_interrupt_enabled()     (Tst_bits(UOTGHS->UOTGHS_CTRL, UOTGHS_CTRL_VBERRE|UOTGHS_CTRL_BCERRE|UOTGHS_CTRL_HNPERRE|UOTGHS_CTRL_STOE))
#define Is_uhd_errors_interrupt()             (Tst_bits(UOTGHS->UOTGHS_SR, UOTGHS_SR_VBERRI|UOTGHS_SR_BCERRI|UOTGHS_SR_HNPERRI|UOTGHS_SR_STOI))

//! @name USB device connection/disconnection monitoring
//! @{
#define uhd_enable_connection_int()           (UOTGHS->UOTGHS_HSTIER = UOTGHS_HSTIER_DCONNIES)
#define uhd_disable_connection_int()          (UOTGHS->UOTGHS_HSTIDR = UOTGHS_HSTIDR_DCONNIEC)
#define Is_uhd_connection_int_enabled()       (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_DCONNIE))
#define uhd_ack_connection()                  (UOTGHS->UOTGHS_HSTICR = UOTGHS_HSTICR_DCONNIC)
#define Is_uhd_connection()                   (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_DCONNI))
#define uhd_raise_connection()                (UOTGHS->UOTGHS_HSTIFR = UOTGHS_HSTIFR_DCONNIS)

#define uhd_enable_disconnection_int()        (UOTGHS->UOTGHS_HSTIER = UOTGHS_HSTIER_DDISCIES)
#define uhd_disable_disconnection_int()       (UOTGHS->UOTGHS_HSTIDR = UOTGHS_HSTIDR_DDISCIEC)
#define Is_uhd_disconnection_int_enabled()    (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_DDISCIE))
#define uhd_ack_disconnection()               (UOTGHS->UOTGHS_HSTICR = UOTGHS_HSTICR_DDISCIC)
#define Is_uhd_disconnection()                (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_DDISCI))
#define uhd_raise_disconnection()             (UOTGHS->UOTGHS_HSTIFR = UOTGHS_HSTIFR_DDISCIS)
//! @}

//! @name USB device speed control
//! @{
#define uhd_get_speed_mode()                  (Rd_bits(UOTGHS->UOTGHS_SR, UOTGHS_SR_SPEED_Msk))
#define Is_uhd_low_speed_mode()                 (uhd_get_speed_mode() == UOTGHS_SR_SPEED_LOW_SPEED)
#define Is_uhd_full_speed_mode()                (uhd_get_speed_mode() == UOTGHS_SR_SPEED_FULL_SPEED)
#define Is_uhd_high_speed_mode()                (uhd_get_speed_mode() == UOTGHS_SR_SPEED_HIGH_SPEED)
//! Enable high speed mode
# define uhd_enable_high_speed_mode()        (Wr_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_SPDCONF_Msk, UOTGHS_HSTCTRL_SPDCONF_HIGH_SPEED))
//! Disable high speed mode
# define uhd_disable_high_speed_mode()       (Wr_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_SPDCONF_Msk, UOTGHS_HSTCTRL_SPDCONF_FORCED_FS))
//! @}

//! @name Bus events control
//! These macros manage the bus events: reset, SOF, resume, wakeup.
//! @{

//! Initiates a reset event
//! @{
#define uhd_start_reset()                            (Set_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_RESET))
#define Is_uhd_starting_reset()                      (Tst_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_RESET))
#define uhd_stop_reset()                             (Clr_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_RESET))

#define uhd_enable_reset_sent_interrupt()            (UOTGHS->UOTGHS_HSTIER = UOTGHS_HSTIER_RSTIES)
#define uhd_disable_reset_sent_interrupt()           (UOTGHS->UOTGHS_HSTIDR = UOTGHS_HSTIDR_RSTIEC)
#define Is_uhd_reset_sent_interrupt_enabled()        (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_RSTIE))
#define uhd_ack_reset_sent()                         (UOTGHS->UOTGHS_HSTICR = UOTGHS_HSTICR_RSTIC)
#define Is_uhd_reset_sent()                          (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_RSTI))
//! @}

//! Initiates a SOF events
//! @{
#define uhd_enable_sof()                             (Set_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_SOFE))
#define uhd_disable_sof()                            (Clr_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_SOFE))
#define Is_uhd_sof_enabled()                         (Tst_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_SOFE))
#define uhd_get_sof_number()                         ((UOTGHS->UOTGHS_HSTFNUM & UOTGHS_HSTFNUM_FNUM_Msk) >> UOTGHS_HSTFNUM_FNUM_Pos)
#define uhd_get_microsof_number()                    ((UOTGHS->UOTGHS_HSTFNUM & \
		(UOTGHS_HSTFNUM_FNUM_Msk|UOTGHS_HSTFNUM_MFNUM_Msk)) >> UOTGHS_HSTFNUM_FNUM_Pos)
#define uhd_get_frame_position()                     (Rd_bitfield(UOTGHS->UOTGHS_HSTFNUM, UOTGHS_HSTFNUM_FLENHIGH_Msk))
#define uhd_enable_sof_interrupt()                   (UOTGHS->UOTGHS_HSTIER = UOTGHS_HSTIER_HSOFIES)
#define uhd_disable_sof_interrupt()                  (UOTGHS->UOTGHS_HSTIDR = UOTGHS_HSTIDR_HSOFIEC)
#define Is_uhd_sof_interrupt_enabled()               (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_HSOFIE))
#define uhd_ack_sof()                                (UOTGHS->UOTGHS_HSTICR = UOTGHS_HSTICR_HSOFIC)
#define Is_uhd_sof()                                 (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_HSOFI))
//! @}

//! Initiates a resume event
//! It is called downstream resume event.
//! @{
#define uhd_send_resume()                            (Set_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_RESUME))
#define Is_uhd_sending_resume()                      (Tst_bits(UOTGHS->UOTGHS_HSTCTRL, UOTGHS_HSTCTRL_RESUME))

#define uhd_enable_downstream_resume_interrupt()     (UOTGHS->UOTGHS_HSTIER = UOTGHS_HSTIER_RSMEDIES)
#define uhd_disable_downstream_resume_interrupt()    (UOTGHS->UOTGHS_HSTIDR = UOTGHS_HSTIDR_RSMEDIEC)
#define Is_uhd_downstream_resume_interrupt_enabled() (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_RSMEDIE))
#define uhd_ack_downstream_resume()                  (UOTGHS->UOTGHS_HSTICR = UOTGHS_HSTICR_RSMEDIC)
#define Is_uhd_downstream_resume()                   (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_RSMEDI))
//! @}

//! Detection of a wake-up event
//! A wake-up event is received when the host controller is in the suspend mode:
//! - and an upstream resume from the peripheral is detected.
//! - and a peripheral disconnection is detected.
//! @{
#define uhd_enable_wakeup_interrupt()                (UOTGHS->UOTGHS_HSTIER = UOTGHS_HSTIER_HWUPIES)
#define uhd_disable_wakeup_interrupt()               (UOTGHS->UOTGHS_HSTIDR = UOTGHS_HSTIDR_HWUPIEC)
#define Is_uhd_wakeup_interrupt_enabled()            (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_HWUPIE))
#define uhd_ack_wakeup()                             (UOTGHS->UOTGHS_HSTICR = UOTGHS_HSTICR_HWUPIC)
#define Is_uhd_wakeup()                              (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_HWUPI))

#define uhd_enable_upstream_resume_interrupt()       (UOTGHS->UOTGHS_HSTIER = UOTGHS_HSTIER_RXRSMIES)
#define uhd_disable_upstream_resume_interrupt()      (UOTGHS->UOTGHS_HSTIDR = UOTGHS_HSTIDR_RXRSMIEC)
#define Is_uhd_upstream_resume_interrupt_enabled()   (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_RXRSMIE))
#define uhd_ack_upstream_resume()                    (UOTGHS->UOTGHS_HSTICR = UOTGHS_HSTICR_RXRSMIC)
#define Is_uhd_upstream_resume()                     (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_RXRSMI))
//! @}
//! @}


//! @name Pipes management
//! @{

//! USB address of pipes
//! @{
#define uhd_configure_address(p, addr) \
		(Wr_bitfield((&UOTGHS->UOTGHS_HSTADDR1)[(p)>>2], \
		UOTGHS_HSTADDR1_HSTADDRP0_Msk << (((p)&0x03)<<3), addr))
#define uhd_get_configured_address(p) \
		(Rd_bitfield((&UOTGHS->UOTGHS_HSTADDR1)[(p)>>2], \
		UOTGHS_HSTADDR1_HSTADDRP0_Msk << (((p)&0x03)<<3)))
//! @}

//! Pipe enable
//! Enable, disable, reset, freeze
//! @{
#define uhd_enable_pipe(p) \
		(Set_bits(UOTGHS->UOTGHS_HSTPIP, UOTGHS_HSTPIP_PEN0 << (p)))
#define uhd_disable_pipe(p) \
		(Clr_bits(UOTGHS->UOTGHS_HSTPIP, UOTGHS_HSTPIP_PEN0 << (p)))
#define Is_uhd_pipe_enabled(p) \
		(Tst_bits(UOTGHS->UOTGHS_HSTPIP, UOTGHS_HSTPIP_PEN0 << (p)))
#define uhd_reset_pipe(p) \
		(Set_bits(UOTGHS->UOTGHS_HSTPIP, UOTGHS_HSTPIP_PRST0 << (p))); \
		(Clr_bits(UOTGHS->UOTGHS_HSTPIP, UOTGHS_HSTPIP_PRST0 << (p)))
#define Is_uhd_resetting_pipe(p) \
		(Tst_bits(UOTGHS->UOTGHS_HSTPIP, UOTGHS_HSTPIP_PRST0 << (p)))
#define uhd_freeze_pipe(p)                       (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_PFREEZES)
#define uhd_unfreeze_pipe(p)                     (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_PFREEZEC)
#define Is_uhd_pipe_frozen(p)                    (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_PFREEZE))
#define uhd_reset_data_toggle(p)                 (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_RSTDTS)
#define Is_uhd_data_toggle_reset(p)              (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_RSTDT))
//! @}

//! Pipe configuration
//! @{
#define uhd_configure_pipe_int_req_freq(p,freq)  (Wr_bitfield(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_INTFRQ_Msk, (freq)))
#define uhd_get_pipe_int_req_freq(p)             (Rd_bitfield(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_INTFRQ_Msk))
#define uhd_configure_pipe_endpoint_number(p,ep) (Wr_bitfield(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PEPNUM_Msk, (ep)))
#define uhd_get_pipe_endpoint_address(p) \
		(uhd_is_pipe_in(p) ?\
			(uhd_get_pipe_endpoint_number(p) | USB_EP_DIR_IN) :\
			(uhd_get_pipe_endpoint_number(p) | USB_EP_DIR_OUT))
#define uhd_get_pipe_endpoint_number(p)          (Rd_bitfield(UOTGHS->UOTGHS_HSTPIPCFG[p], (UOTGHS_HSTPIPCFG_PEPNUM_Msk)))
#define uhd_configure_pipe_type(p, type)         (Wr_bitfield(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PTYPE_Msk, type))
#define uhd_get_pipe_type(p)                     (Rd_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PTYPE_Msk))
#define Is_uhd_pipe_int(p)  ((UOTGHS->UOTGHS_HSTPIPCFG[p] & UOTGHS_HSTPIPCFG_PTYPE_Msk)==UOTGHS_HSTPIPCFG_PTYPE_INTRPT)
#define Is_uhd_pipe_iso(p)  ((UOTGHS->UOTGHS_HSTPIPCFG[p] & UOTGHS_HSTPIPCFG_PTYPE_Msk)==UOTGHS_HSTPIPCFG_PTYPE_ISO)
#define Is_uhd_pipe_bulk(p) ((UOTGHS->UOTGHS_HSTPIPCFG[p] & UOTGHS_HSTPIPCFG_PTYPE_Msk)==UOTGHS_HSTPIPCFG_PTYPE_BLK)
#define Is_uhd_pipe_bulk_in(p) ((UOTGHS->UOTGHS_HSTPIPCFG[p] & (UOTGHS_HSTPIPCFG_PTYPE_Msk|UOTGHS_HSTPIPCFG_PTOKEN_Msk))==(UOTGHS_HSTPIPCFG_PTYPE_BLK|UOTGHS_HSTPIPCFG_PTOKEN_IN))
#define uhd_enable_pipe_bank_autoswitch(p)       (Set_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_AUTOSW))
#define uhd_disable_pipe_bank_autoswitch(p)      (Clr_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_AUTOSW))
#define Is_uhd_pipe_bank_autoswitch_enabled(p)   (Tst_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_AUTOSW))
#define uhd_configure_pipe_token(p, token)       (Wr_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PTOKEN_Msk, token))
#define uhd_get_pipe_token(p)                    (Rd_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PTOKEN_Msk))
#define uhd_is_pipe_in(p)                        (UOTGHS_HSTPIPCFG_PTOKEN_IN==uhd_get_pipe_token(p))
#define uhd_is_pipe_out(p)                       (UOTGHS_HSTPIPCFG_PTOKEN_OUT==uhd_get_pipe_token(p))
#define Is_uhd_pipe_in(p)                        (UOTGHS_HSTPIPCFG_PTOKEN_IN==uhd_get_pipe_token(p))
#define Is_uhd_pipe_out(p)                       (UOTGHS_HSTPIPCFG_PTOKEN_OUT==uhd_get_pipe_token(p))
//! Bounds given integer size to allowed range and rounds it up to the nearest
//! available greater size, then applies register format of UOTGHS controller
//! for pipe size bit-field.
#define uhd_format_pipe_size(size) \
		(32 - clz(((uint32_t)min(max(size, 8), 1024) << 1) - 1) - 1 - 3)
#define uhd_configure_pipe_size(p,size) \
		(Wr_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PSIZE_Msk, uhd_format_pipe_size(size)))
#define uhd_get_pipe_size(p)                     (8<<((Rd_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], (UOTGHS_HSTPIPCFG_PSIZE_Msk)))>> UOTGHS_HSTPIPCFG_PSIZE_Pos))
#define uhd_configure_pipe_bank(p,bank)          (Wr_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PBK_Msk, (bank)))
#define uhd_get_pipe_bank(p)                     (Rd_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PBK_Msk))
#define uhd_allocate_memory(p)                   (Set_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_ALLOC))
#define uhd_unallocate_memory(p)                 (Clr_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_ALLOC))
#define Is_uhd_memory_allocated(p)               (Tst_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_ALLOC))

//! Enable PING management only available in HS mode
#  define uhd_enable_ping(p)                     (Set_bits(UOTGHS->UOTGHS_HSTPIPCFG[p], UOTGHS_HSTPIPCFG_PINGEN))

#define uhd_configure_pipe(p, freq, ep_num, type, token, size, bank, bank_switch) \
	(Set_bits(UOTGHS->UOTGHS_HSTPIPCFG[p],\
		(bank)|\
		((uhd_format_pipe_size(size)<<UOTGHS_HSTPIPCFG_PSIZE_Pos)&UOTGHS_HSTPIPCFG_PSIZE_Msk)|\
		(((token)<<UOTGHS_HSTPIPCFG_PTOKEN_Pos)&UOTGHS_HSTPIPCFG_PTOKEN_Msk)|\
		(((type)<<UOTGHS_HSTPIPCFG_PTYPE_Pos)&UOTGHS_HSTPIPCFG_PTYPE_Msk)|\
		(((ep_num)<<UOTGHS_HSTPIPCFG_PEPNUM_Pos)&UOTGHS_HSTPIPCFG_PEPNUM_Msk)|\
		bank_switch |\
		(((freq)<<UOTGHS_HSTPIPCFG_INTFRQ_Pos)&UOTGHS_HSTPIPCFG_INTFRQ_Msk)))

#define Is_uhd_pipe_configured(p)          (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_CFGOK))
//! @}

//! Pipe main interrupts management
//! @{
#define uhd_enable_pipe_interrupt(p)       (UOTGHS->UOTGHS_HSTIER = (UOTGHS_HSTIER_PEP_0 << (p)))
#define uhd_disable_pipe_interrupt(p)      (UOTGHS->UOTGHS_HSTIDR = (UOTGHS_HSTIDR_PEP_0 << (p)))
#define Is_uhd_pipe_interrupt_enabled(p)   (Tst_bits(UOTGHS->UOTGHS_HSTIMR, UOTGHS_HSTIMR_PEP_0 << (p)))
#define Is_uhd_pipe_interrupt(p)           (Tst_bits(UOTGHS->UOTGHS_HSTISR, UOTGHS_HSTISR_PEP_0 << (p)))
//! returns the lowest pipe number generating a pipe interrupt or UOTGHS_EPT_NUM if none
static inline uint8_t uhd_get_interrupt_pipe_number(void)
{
	uint32_t status = UOTGHS->UOTGHS_HSTISR;
	uint32_t mask = UOTGHS->UOTGHS_HSTIMR;
	return ctz(((status & mask) >> 8) | (1 << UOTGHS_EPT_NUM));
}
//! @}

//! Pipe overflow and underflow for isochronous and interrupt endpoints
//! @{
#define uhd_enable_overflow_interrupt(p)         (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_OVERFIES)
#define uhd_disable_overflow_interrupt(p)        (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_OVERFIEC)
#define Is_uhd_overflow_interrupt_enabled(p)     (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_OVERFIE))
#define uhd_ack_overflow_interrupt(p)            (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_OVERFIC)
#define Is_uhd_overflow(p)                       (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_OVERFI))

#define uhd_enable_underflow_interrupt(p)        (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_UNDERFIES)
#define uhd_disable_underflow_interrupt(p)       (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_UNDERFIEC)
#define Is_uhd_underflow_interrupt_enabled(p)    (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_UNDERFIE))
#define uhd_ack_underflow_interrupt(p)           (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_UNDERFIC)
#define Is_uhd_underflow(p)                      (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_UNDERFI))
//! @}

//! USB packet errors management
//! @{
#define uhd_enable_stall_interrupt(p)            (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_RXSTALLDES)
#define uhd_disable_stall_interrupt(p)           (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_RXSTALLDEC)
#define Is_uhd_stall_interrupt_enabled(p)        (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_RXSTALLDE))
#define uhd_ack_stall(p)                         (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_RXSTALLDIC)
#define Is_uhd_stall(p)                          (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_RXSTALLDI))

#define uhd_enable_pipe_error_interrupt(p)       (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_PERRES)
#define uhd_disable_pipe_error_interrupt(p)      (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_PERREC)
#define Is_uhd_pipe_error_interrupt_enabled(p)   (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_PERRE))
#define uhd_ack_all_errors(p)                    (UOTGHS->UOTGHS_HSTPIPERR[p] = 0UL)
#define Is_uhd_pipe_error(p)                     (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_PERRI))
#define uhd_error_status(p)                      (UOTGHS->UOTGHS_HSTPIPERR[p])
#define Is_uhd_bad_data_toggle(p)                (Tst_bits(UOTGHS->UOTGHS_HSTPIPERR[p], UOTGHS_HSTPIPERR_DATATGL))
#define Is_uhd_data_pid_error(p)                 (Tst_bits(UOTGHS->UOTGHS_HSTPIPERR[p], UOTGHS_HSTPIPERR_DATAPID))
#define Is_uhd_pid_error(p)                      (Tst_bits(UOTGHS->UOTGHS_HSTPIPERR[p], UOTGHS_HSTPIPERR_PID))
#define Is_uhd_timeout_error(p)                  (Tst_bits(UOTGHS->UOTGHS_HSTPIPERR[p], UOTGHS_HSTPIPERR_TIMEOUT))
#define Is_uhd_crc16_error(p)                    (Tst_bits(UOTGHS->UOTGHS_HSTPIPERR[p], UOTGHS_HSTPIPERR_CRC16))
#define uhd_get_error_counter(p)                 (Rd_bits(UOTGHS->UOTGHS_HSTPIPERR[p], UOTGHS_HSTPIPERR_COUNTER))
//! @}

//! Pipe data management
//! @{
#define uhd_data_toggle(p)                       (Rd_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_DTSEQ))

#define uhd_enable_bank_interrupt(p)             (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_NBUSYBKES)
#define uhd_disable_bank_interrupt(p)            (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_NBUSYBKEC)
#define Is_uhd_bank_interrupt_enabled(p)         (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_NBUSYBKE))
#define uhd_nb_busy_bank(p)                      (Rd_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_NBUSYBK_Msk))
#define uhd_current_bank(p)                      (Rd_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_CURRBK_Msk))

#define uhd_enable_short_packet_interrupt(p)     (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_SHORTPACKETIES)
#define uhd_disable_short_packet_interrupt(p)    (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_SHORTPACKETIEC)
#define Is_uhd_short_packet_interrupt_enabled(p) (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_SHORTPACKETIE))
#define uhd_ack_short_packet(p)                  (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_SHORTPACKETIC)
#define Is_uhd_short_packet(p)                   (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_SHORTPACKETI))
#define uhd_byte_count(p)                        (Rd_bitfield(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_PBYCT_Msk))

#define Is_uhd_fifocon(p)                        (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_FIFOCON))
#define uhd_ack_fifocon(p)                       (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_FIFOCONC)

#define uhd_enable_setup_ready_interrupt(p)      (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_TXSTPES)
#define uhd_disable_setup_ready_interrupt(p)     (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_TXSTPEC)
#define Is_uhd_setup_ready_interrupt_enabled(p)  (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_TXSTPE))
#define uhd_ack_setup_ready(p)                   (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_TXSTPIC)
#define Is_uhd_setup_ready(p)                    (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_TXSTPI))

#define uhd_enable_in_received_interrupt(p)      (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_RXINES)
#define uhd_disable_in_received_interrupt(p)     (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_RXINEC)
#define Is_uhd_in_received_interrupt_enabled(p)  (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_RXINE))
#define uhd_ack_in_received(p)                   (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_RXINIC)
#define Is_uhd_in_received(p)                    (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_RXINI))

#define uhd_enable_out_ready_interrupt(p)        (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_TXOUTES)
#define uhd_disable_out_ready_interrupt(p)       (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_TXOUTEC)
#define Is_uhd_out_ready_interrupt_enabled(p)    (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_TXOUTE))
#define uhd_ack_out_ready(p)                     (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_TXOUTIC)
#define Is_uhd_out_ready(p)                      (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_TXOUTI))
#define uhd_raise_out_ready(p)                   (UOTGHS->UOTGHS_HSTPIPIFR[p] = UOTGHS_HSTPIPIFR_TXOUTIS)

#define uhd_enable_nak_received_interrupt(p)     (UOTGHS->UOTGHS_HSTPIPIER[p] = UOTGHS_HSTPIPIER_NAKEDES)
#define uhd_disable_nak_received_interrupt(p)    (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_NAKEDEC)
#define Is_uhd_nak_received_interrupt_enabled(p) (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_NAKEDE))
#define uhd_ack_nak_received(p)                  (UOTGHS->UOTGHS_HSTPIPICR[p] = UOTGHS_HSTPIPICR_NAKEDIC)
#define Is_uhd_nak_received(p)                   (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_NAKEDI))

#define Is_uhd_read_enabled(p)                   (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_RWALL))
#define Is_uhd_write_enabled(p)                  (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTPIPISR_RWALL))

#define uhd_enable_continuous_in_mode(p)         (Set_bits(UOTGHS->UOTGHS_HSTPIPINRQ[p], UOTGHS_HSTPIPINRQ_INMODE))
#define uhd_disable_continuous_in_mode(p)        (Clr_bits(UOTGHS->UOTGHS_HSTPIPINRQ[p], UOTGHS_HSTPIPINRQ_INMODE))
#define Is_uhd_continuous_in_mode_enabled(p)     (Tst_bits(UOTGHS->UOTGHS_HSTPIPINRQ[p], UOTGHS_HSTPIPINRQ_INMODE))

#define uhd_in_request_number(p, in_num)         (Wr_bits(UOTGHS->UOTGHS_HSTPIPINRQ[p], UOTGHS_HSTPIPINRQ_INRQ_Msk, (in_num)-1))
#define uhd_get_in_request_number(p)             (((Rd_bits(UOTGHS->UOTGHS_HSTPIPINRQ[p], UOTGHS_HSTPIPINRQ_INRQ_Msk))>>UOTGHS_HSTPIPINRQ_INRQ_Pos)+1)
//! @}

//! Pipe DMA management
//! @{

//! Maximum transfer size on USB DMA
#define UHD_PIPE_MAX_TRANS 0x8000

  //! Get 64-, 32-, 16- or 8-bit access to FIFO data register of selected pipe.
  //! @param p      Target Pipe number
  //! @param scale  Data scale in bits: 64, 32, 16 or 8
  //! @return       Volatile 64-, 32-, 16- or 8-bit data pointer to FIFO data register
  //! @warning It is up to the user of this macro to make sure that all accesses
  //! are aligned with their natural boundaries except 64-bit accesses which
  //! require only 32-bit alignment.
  //! @warning It is up to the user of this macro to make sure that used HSB
  //! addresses are identical to the DPRAM internal pointer modulo 32 bits.
#define uhd_get_pipe_fifo_access(p, scale) \
	(((volatile TPASTE2(U, scale) (*)[UHD_PIPE_MAX_TRANS / ((scale) / 8)])UOTGHS_RAM_ADDR)[(p)])

#define uhd_enable_pipe_int_dis_hdma_req(p)      (UOTGHS->UOTGHS_HSTPIPIMR[p] = UOTGHS_HSTPIPIMR_PDISHDMA)
#define uhd_disable_pipe_int_dis_hdma_req(p)     (UOTGHS->UOTGHS_HSTPIPIDR[p] = UOTGHS_HSTPIPIDR_PDISHDMAC)
#define Is_uhd_pipe_int_dis_hdma_req_enabled(p)  (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTPIPIMR_PDISHDMA))

#define uhd_enable_pipe_dma_interrupt(p)         (UOTGHS->UOTGHS_HSTIER = (UOTGHS_HSTIER_DMA_1<<((p)-1)))
#define uhd_disable_pipe_dma_interrupt(p)        (UOTGHS->UOTGHS_HSTIDR = (UOTGHS_HSTIDR_DMA_1<<((p)-1)))
#define Is_uhd_pipe_dma_interrupt_enabled(p)     (Tst_bits(UOTGHS->UOTGHS_HSTPIPIMR[p], UOTGHS_HSTIMR_DMA_1<<((p)-1)))
#define Is_uhd_pipe_dma_interrupt(p)             (Tst_bits(UOTGHS->UOTGHS_HSTPIPISR[p], UOTGHS_HSTISR_DMA_1<<((p)-1)))

static inline uint8_t uhd_get_pipe_dma_interrupt_number(void)
{
	uint32_t status = UOTGHS->UOTGHS_HSTISR;
	uint32_t mask = UOTGHS->UOTGHS_HSTIMR;
	return (ctz(((status & mask) >> 25) | (1 << (UOTGHS_EPT_NUM-1))) + 1);
}

  //! Access points to the UOTGHS host DMA memory map with arrayed registers
  //! @{
      //! Structure for DMA next descriptor register
typedef struct {
	uint32_t *NXT_DSC_ADD;
} uotghs_uhdma_nextdesc_t;

      //! Structure for DMA control register
typedef struct {
	uint32_t CHANN_ENB:1,
		LDNXT_DSC:1,
		END_TR_EN:1,
		END_B_EN:1,
		END_TR_IT:1,
		END_BUFFIT:1,
		DESC_LD_IT:1,
		BUST_LCK:1,
		reserved:8,
		BUFF_LENGTH:16;
} uotghs_uhdma_control_t;
      //! Structure for DMA status register
typedef struct {
	uint32_t CHANN_ENB:1,
		CHANN_ACT:1,
		reserved0:2,
		END_TR_ST:1,
		END_BF_ST:1,
		DESC_LDST:1,
		reserved1:9,
		BUFF_COUNT:16;
} uotghs_uhdma_status_t;
      //! Structure for DMA descriptor
typedef struct {
	union {
		uint32_t nextdesc;
		uotghs_uhdma_nextdesc_t NEXTDESC;
	};
	uint32_t addr;
	union {
		uint32_t control;
		uotghs_uhdma_control_t CONTROL;
	};
	uint32_t reserved;
} sam_uotghs_uhdmadesc_t, uotghs_uhdmadesc_t;
      //! Structure for DMA registers in a channel
typedef struct {
	union {
		uint32_t nextdesc;
		uotghs_uhdma_nextdesc_t NEXTDESC;
	};
	uint32_t addr;
	union {
		uint32_t control;
		uotghs_uhdma_control_t CONTROL;
	};
	union {
		unsigned long status;
		uotghs_uhdma_status_t STATUS;
	};
} sam_uotghs_uhdmach_t, uotghs_uhdmach_t;
      //! DMA channel control command
#define  UHD_ENDPOINT_DMA_STOP_NOW             (0)
#define  UHD_ENDPOINT_DMA_RUN_AND_STOP         (UOTGHS_HSTDMACONTROL_CHANN_ENB)
#define  UHD_ENDPOINT_DMA_LOAD_NEXT_DESC       (UOTGHS_HSTDMACONTROL_LDNXT_DSC)
#define  UHD_ENDPOINT_DMA_RUN_AND_LINK         (UOTGHS_HSTDMACONTROL_CHANN_ENB|UOTGHS_HSTDMACONTROL_LDNXT_DSC)
//! Structure for DMA registers
#define  UOTGHS_UHDMA_ARRAY(ep)                (((volatile uotghs_uhdmach_t *)UOTGHS->UOTGHS_HSTDMA)[(ep) - 1])

//! Set control desc to selected endpoint DMA channel
#define  uhd_pipe_dma_set_control(p,desc)     (UOTGHS_UHDMA_ARRAY(p).control=desc)
//! Get control desc to selected endpoint DMA channel
#define  uhd_pipe_dma_get_control(p)          (UOTGHS_UHDMA_ARRAY(p).control)
//! Set RAM address to selected endpoint DMA channel
#define  uhd_pipe_dma_set_addr(p,add)         (UOTGHS_UHDMA_ARRAY(p).addr=add)
//! Get RAM address from selected endpoint DMA channel
#define  uhd_pipe_dma_get_addr(p)             (UOTGHS_UHDMA_ARRAY(p).addr)
//! Get status to selected endpoint DMA channel
#define  uhd_pipe_dma_get_status(p)           (UOTGHS_UHDMA_ARRAY(p).status)
//! @}
//! @}

//! @}
//@}


#ifdef __cplusplus
}
#endif


#endif // UOTGHS_HOST_H_INCLUDED
