/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <api/globals.h>
#include "wpan.h"
#include <main/managers/widgetsmanager.h>

#include <api/iwidgetsrenderer.h>
#include <api/westilo.h>
#include <vtkgl.h>
#include <vtkSmartPointer.h>

#include <cmath>

#include <vtk/vtkginkgoimageviewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>

//region "Implementacion de WPanBuilder"
#include <api/icontexto.h>


GNC::GCS::Widgets::WPanBuilder::WPanBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
        m_MouseDown = false;
        m_Estado = WBS_Ninguno;
        m_Dentro = true;
        m_Changed = false;
}

GNC::GCS::Widgets::WPanBuilder::~WPanBuilder()
{
}

void GNC::GCS::Widgets::WPanBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{

        if (!m_pManager) {
                return;
        }
        if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
                m_MouseDown = false;
                if (m_Estado != WBS_Creando) {
                        m_Estado = WBS_Ninguno;
                        return;
                }
                vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
                vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());


                if (is != NULL) {
                        is->EndPan();
                }

                if (m_Changed) {
                        ConsumirEvento();
                }

                m_Estado = WBS_Ninguno;
                //m_pManager->Modificado();
                //no se consume el evento para poder mostrar las propiedades de un widget
        } else if (evento.ButtonDown(m_ButtonMask)) {
                if (m_Estado != WBS_Ninguno) {
                        return;
                }
                m_Start = evento.iP;
                m_Changed = false;
                m_MouseDown = true;

                vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
                vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());

                if (is != NULL) {
                        is->StartPan();
                }
                m_Estado = WBS_Creando;
                ConsumirEvento();
        } else if (evento.Dragging() && m_MouseDown) {
                if (m_Estado != WBS_Creando) {
                        return;
                }
                vtkSmartPointer<vtkRenderWindowInteractor> rwi = evento.c->pRenderer->m_pImageViewer->GetRenderWindowInteractor();
                vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());

                if (is != NULL) {
                        is->Pan();
                        is->PropagateCameraFocalAndPosition();
                        if (m_Start.DistanciaEuclideaCuadrado(evento.iP) > TVector::EpsilonDeReales()) {
                                m_Changed = true;
                        }
                }

                //m_pManager->Modificado();
                ConsumirEvento();
        } else if( evento.Moving() ) {
                GTRACE("Moviendose");
                m_NodoMoviendose = evento.iP;
                m_Estado = WBS_Ninguno;
        }

}

void GNC::GCS::Widgets::WPanBuilder::OnKeyEvents(TEventoTeclado&)
{

}

void GNC::GCS::Widgets::WPanBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

        /*
        TVector box = TVector(100, 10) * c->RefRelacionImagenPantalla();

        box = TVector(100, 10) * c->RefRelacionImagenPantalla();
        TVector pos = TVector(std::max(m_Start.x - box.x, std::min(m_Start.x + box.x, m_Stop.x)), m_Start.y);

        if (m_Estado == WBS_Creando) {
        	glLineWidth(2.0f);
        	glColor4f(0.0f, 0.75f, 0.0f, 1.0f);

        	glBegin(GL_LINES);

        		glVertex2d(m_Start.x - box.x, m_Start.y);
        		glVertex2d(m_Start.x + box.x, m_Start.y);

        		glVertex2d(m_Start.x - box.x, m_Start.y - box.y);
        		glVertex2d(m_Start.x - box.x, m_Start.y + box.y);

        		glVertex2d(m_Start.x + box.x, m_Start.y - box.y);
        		glVertex2d(m_Start.x + box.x, m_Start.y + box.y);

        	glEnd();

        	glColor4f(0.75f, 0.0f, 0.0f, 1.0f);

        	glLineWidth(1.5);
        	glBegin(GL_LINES);
        		glVertex2d(pos.x, pos.y - box.y);
        		glVertex2d(pos.x, pos.y + box.y);
        	glEnd();
        }
        else {

        }
        */
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WPanBuilder::GetCursor()
{
        if (m_Estado == WBS_Creando && m_Changed) {
                return GNC::GCS::Widgets::CUR_MANO_CERRADA;
        } else {
                return GNC::GCS::Widgets::CUR_MANO_ABIERTA;
        }
}

//endregion
