from django.conf.urls import url

from . import views

urlpatterns = [
    url(r'^execute$', views.execute, name='execute'),
    url(r'^$', views.login, name='login'),
    url(r'^login$', views.login, name='login'),
    url(r'^login.verify$', views.login_verify),
    url(r'^logout$', views.logout, name='logout'),
    url(r'^client_execute$', views.client_execute, name='ce'),
]
