import asyncio

import click

from leosacpy.exception import APIError
from leosacpy.utils import AWAIT
from leosacpy.ws import LeosacMessage, APIStatusCode
from leosacpy.wsclient import LowLevelWSClient, LeosacAPI


@click.group('pfdigital')
@click.option('--host', '-h')
@click.pass_context
def pfdigital_cmd_group(ctx, host):
    """
    Command group to interact with piface digital module.

    """
    if host:
        ctx.obj.config.host = host


@pfdigital_cmd_group.command('gpio-list')
@click.pass_context
def list_gpio(ctx):
    host = ctx.obj.config.host
    username = ctx.obj.config.username
    password = ctx.obj.config.password

    loop = asyncio.new_event_loop()
    api = LeosacAPI(host)
    AWAIT(api.authenticate(username, password), loop)

    msg = LeosacMessage('pfdigital.gpio.read', {'gpio_id': 0})
    rep = AWAIT(api._req_rep(msg), loop)
    for gpio in rep.content['data']:
        print(gpio)

    AWAIT(api.close(), loop)


@pfdigital_cmd_group.command('gpio-create')
@click.option('--name', required=True)
@click.option('--number')
@click.option('--default')
@click.option('--direction')
@click.option('--hardware_address')
@click.pass_context
def create_gpio(ctx, name, number, default, direction, hardware_address):
    host = ctx.obj.config.host
    username = ctx.obj.config.username
    password = ctx.obj.config.password

    loop = asyncio.new_event_loop()
    api = LeosacAPI(host)
    AWAIT(api.authenticate(username, password), loop)

    msg = LeosacMessage('pfdigital.gpio.create',
                        {'attributes': {
                            'name': name,
                            'number': number,
                            'default': default,
                            'direction': direction,
                            'hardware_address': hardware_address
                        }})
    rep = AWAIT(api._req_rep(msg), loop)
    if rep.status_code == APIStatusCode.SUCCESS:
        click.echo('Created GPIO {}'.format(rep.content['data']['id']))
    else:
        click.echo('Failed to create GPIO ! Status code: {}'.format(rep.status_code))
    AWAIT(api.close(), loop)


@pfdigital_cmd_group.command('gpio-test')
@click.option('--id', required=True, type=int)
@click.pass_context
def test_gpio(ctx, id):
    """
    Make a GPIO blink in order to test it
    :return:
    """
    host = ctx.obj.config.host
    username = ctx.obj.config.username
    password = ctx.obj.config.password

    loop = asyncio.new_event_loop()
    api = LeosacAPI(host)
    AWAIT(api.authenticate(username, password), loop)

    msg = LeosacMessage('pfdigital.test_output_pin',
                        {'gpio_id': id})

    try:
        rep = AWAIT(api._req_rep(msg), loop)
        if rep.status_code == APIStatusCode.SUCCESS:
            click.echo('Starting test of GPIO !')
    except APIError as e:
        click.echo('API Error: Status Code: {}. Message: {}'.format(
            e.status_code(), e.status_string()))
    AWAIT(api.close(), loop)
