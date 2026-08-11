/*! @file
	@brief モダンUI(Windows 11 世代の外観)ヘルパー

	DWM の Windows 11 向けウィンドウ属性をラップし、
	モダンUI設定の適用を一箇所に集約する。

	キャプション色・境界色はダークモード対応 (darkmodelib の
	DarkMode::setDarkTitleBarEx) が既に面倒を見ているので、
	ここでは扱わない。二重に設定すると競合するため。

	SPDX-License-Identifier: Zlib
*/
#pragma once

#include <dwmapi.h>

namespace ModernUI {

/*! DWM ウィンドウ属性の値

	dwmapi.h の DWMWINDOWATTRIBUTE 列挙は Windows SDK のバージョンによって
	これらの値を持たないことがある。ビルド環境の SDK に依存しないよう、
	値を自前で保持する (列挙子名と衝突しないよう別名にしてある)。
*/
inline constexpr DWORD kAttrWindowCornerPreference = 33;	//!< DWMWA_WINDOW_CORNER_PREFERENCE
inline constexpr DWORD kAttrSystemBackdropType     = 38;	//!< DWMWA_SYSTEMBACKDROP_TYPE

//! DWM_WINDOW_CORNER_PREFERENCE
inline constexpr DWORD kCornerDefault = 0;	//!< DWMWCP_DEFAULT   OSに任せる
inline constexpr DWORD kCornerRound   = 2;	//!< DWMWCP_ROUND     角を丸める

//! DWM_SYSTEMBACKDROP_TYPE
inline constexpr DWORD kBackdropAuto       = 0;	//!< DWMSBT_AUTO         OSに任せる
inline constexpr DWORD kBackdropMainWindow = 2;	//!< DWMSBT_MAINWINDOW   Mica

/*! モダンUI設定をウィンドウに反映する

	Windows 10 以前や Windows 11 22H2 未満では DwmSetWindowAttribute が
	E_INVALIDARG を返すだけで、ウィンドウには何も起きない。
	その環境では単に従来の外観のままになるのが正しい挙動なので、
	戻り値は意図的に無視する。

	@param hWnd       対象ウィンドウ
	@param bModernUI  共有データの m_bModernUI 値
*/
inline void ApplyToWindow(HWND hWnd, BOOL bModernUI)
{
	if (!hWnd) {
		return;
	}

	// 角丸
	const DWORD dwCorner = bModernUI ? kCornerRound : kCornerDefault;
	::DwmSetWindowAttribute(hWnd, kAttrWindowCornerPreference, &dwCorner, sizeof(dwCorner));

	// Mica
	// 編集領域 (CEditView) は背景を不透明に塗り潰すので、
	// 実際に Mica が見えるのはタイトルバー周辺に限られる。
	const DWORD dwBackdrop = bModernUI ? kBackdropMainWindow : kBackdropAuto;
	::DwmSetWindowAttribute(hWnd, kAttrSystemBackdropType, &dwBackdrop, sizeof(dwBackdrop));
}

}	// namespace ModernUI
