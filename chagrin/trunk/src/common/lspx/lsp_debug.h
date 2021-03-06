#ifndef _LSP_DEBUG_H_
#define _LSP_DEBUG_H_
#ifdef LSPIMP_DEBUG_UMODE
#define LSPIMP_DEBUG

void
lsp_call
lsp_debug(const char* format, ...);

void
lsp_call
lsp_debug_payload(const char* header, const void* buf, size_t len);

#else
#define lsp_debug_payload __noop
#define lsp_debug __noop
#endif /* LSPIMP_DEBUG_UMODE */

#endif /* _LSP_DEBUG_H_ */
