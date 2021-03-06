/*++

Copyright (C)2002-2005 XIMETA, Inc.
All rights reserved.

--*/

#include "precomp.h"
#pragma hdrstop


NTSTATUS
LpxTdiSetEventHandler(
    IN PIRP Irp
    )

/*++

Routine Description:

    This routine performs the TdiSetEventHandler request for the
    transport provider.  The caller (request dispatcher) verifies
    that this routine will not be executed on behalf of a user-mode
    client, as this request enables direct callouts at DISPATCH_LEVEL.

Arguments:

    Irp - Pointer to the IRP for this request

Return Value:

    NTSTATUS - status of operation.

--*/

{
    NTSTATUS rc=STATUS_SUCCESS;
    KIRQL oldirql;
    PTDI_REQUEST_KERNEL_SET_EVENT parameters;
    PIO_STACK_LOCATION irpSp;
    PTP_ADDRESS address;
    PTP_ADDRESS_FILE addressFile;
    NTSTATUS status;

    //
    // Get the Address this is associated with; if there is none, get out.
    //

    irpSp = IoGetCurrentIrpStackLocation (Irp);

    if (irpSp->FileObject->FsContext2 != (PVOID) TDI_TRANSPORT_ADDRESS_FILE) {
        return STATUS_INVALID_ADDRESS;
    }

    addressFile  = irpSp->FileObject->FsContext;
    status = LpxVerifyAddressObject (addressFile);
    if (!NT_SUCCESS (status)) {
        return status;
    }

    address = addressFile->Address;

    ACQUIRE_SPIN_LOCK (&address->SpinLock, &oldirql);

    parameters = (PTDI_REQUEST_KERNEL_SET_EVENT)&irpSp->Parameters;

    switch (parameters->EventType) {

    case TDI_EVENT_RECEIVE:

        if (parameters->EventHandler == NULL) {
            addressFile->ReceiveHandler =
                (PTDI_IND_RECEIVE)TdiDefaultReceiveHandler;
            addressFile->ReceiveHandlerContext = NULL;
            addressFile->RegisteredReceiveHandler = FALSE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set STREAM DefaultReceiveHandler.\n");
            }

        } else {
            addressFile->ReceiveHandler =
                (PTDI_IND_RECEIVE)parameters->EventHandler;
            addressFile->ReceiveHandlerContext = parameters->EventContext;
            addressFile->RegisteredReceiveHandler = TRUE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set STREAM User ReceiveHandler.\n");
            }
        }

        break;

    case TDI_EVENT_RECEIVE_EXPEDITED:

        if (parameters->EventHandler == NULL) {
            addressFile->ExpeditedDataHandler =
                (PTDI_IND_RECEIVE_EXPEDITED)TdiDefaultRcvExpeditedHandler;
            addressFile->ExpeditedDataHandlerContext = NULL;
            addressFile->RegisteredExpeditedDataHandler = FALSE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set Default ReceiveEXPEDITEDHandler.\n");
            }
        } else {
            addressFile->ExpeditedDataHandler =
                (PTDI_IND_RECEIVE_EXPEDITED)parameters->EventHandler;
            addressFile->ExpeditedDataHandlerContext = parameters->EventContext;
            addressFile->RegisteredExpeditedDataHandler = TRUE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set User ReceiveEXPEDITEDHandler.\n");
            }
        }

        break;

    case TDI_EVENT_RECEIVE_DATAGRAM:

        if (parameters->EventHandler == NULL) {
            addressFile->ReceiveDatagramHandler =
                (PTDI_IND_RECEIVE_DATAGRAM)TdiDefaultRcvDatagramHandler;
            addressFile->ReceiveDatagramHandlerContext = NULL;
            addressFile->RegisteredReceiveDatagramHandler = FALSE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set ReceiveDefaultDatagramHandler.\n");
            }
        } else {
            addressFile->ReceiveDatagramHandler =
                (PTDI_IND_RECEIVE_DATAGRAM)parameters->EventHandler;
            addressFile->ReceiveDatagramHandlerContext = parameters->EventContext;
            addressFile->RegisteredReceiveDatagramHandler = TRUE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set User ReceiveEDatagramHandler.\n");
            }
        }

        break;

    case TDI_EVENT_ERROR:

        if (parameters->EventHandler == NULL) {
            addressFile->ErrorHandler =
                (PTDI_IND_ERROR)TdiDefaultErrorHandler;
            addressFile->ErrorHandlerContext = NULL;
            addressFile->RegisteredErrorHandler = FALSE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set ErrorDefaultDatagramHandler.\n");
            }

        } else {
            addressFile->ErrorHandler =
                (PTDI_IND_ERROR)parameters->EventHandler;
            addressFile->ErrorHandlerContext = parameters->EventContext;
            addressFile->RegisteredErrorHandler = TRUE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set User ErrorDatagramHandler.\n");
            }
        }

        break;

    case TDI_EVENT_DISCONNECT:

        if (parameters->EventHandler == NULL) {
            addressFile->DisconnectHandler =
                (PTDI_IND_DISCONNECT)TdiDefaultDisconnectHandler;
            addressFile->DisconnectHandlerContext = NULL;
            addressFile->RegisteredDisconnectHandler = FALSE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set Default DisconnectHandler.\n");
            }

        } else {
            addressFile->DisconnectHandler =
                (PTDI_IND_DISCONNECT)parameters->EventHandler;
            addressFile->DisconnectHandlerContext = parameters->EventContext;
            addressFile->RegisteredDisconnectHandler = TRUE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set User DisconnectHandler.\n");
            }

        }

        break;

    case TDI_EVENT_CONNECT:

        if (parameters->EventHandler == NULL) {
            addressFile->ConnectionHandler =
                (PTDI_IND_CONNECT)TdiDefaultConnectHandler;
            addressFile->ConnectionHandlerContext = NULL;
            addressFile->RegisteredConnectionHandler = FALSE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set Default ConnectHandler.\n");
            }
        } else {
            addressFile->ConnectionHandler =
                (PTDI_IND_CONNECT)parameters->EventHandler;
            addressFile->ConnectionHandlerContext = parameters->EventContext;
            addressFile->RegisteredConnectionHandler = TRUE;

            IF_LPXDBG (LPX_DEBUG_DISPATCH) {
                LpxPrint0 ("LpxTdiSetEventHandler: set User ConnectHandler.\n");
            }
        }
            break;

    default:
		IF_LPXDBG (LPX_DEBUG_DISPATCH) {
			LpxPrint1("LpxTdiSetEventHandler: EventType not supported: %d\n", parameters->EventType) ;
		}

        rc = STATUS_INVALID_PARAMETER;

    } /* switch */

    RELEASE_SPIN_LOCK (&address->SpinLock, oldirql);

    LpxDereferenceAddress ("Set event handler", address, AREF_VERIFY);

    return rc;
} /* TdiSetEventHandler */
